/*
 * Copyright (C) 2015 Edward Baudrez <edward.baudrez@gmail.com>
 * This file is part of h5gen.
 *
 * h5gen is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * h5gen is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with h5gen; if not, see <http://www.gnu.org/licenses/>.
 */

#include <config.h>
#include "node.h"
#include "log.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <hdf5.h>
#include "sglib.h"

/* root node of the tree */
node_t *file = NULL;

node_t *
node_new_file(char *name, node_t *root_group)
{
    node_t *node;
    assert(node = malloc(sizeof *node));
    node->type = NODE_FILE;
    node->id = -1;
    node->u.file.name = name; /* no need to strdup(); storage already allocated by unquote() */
    node->u.file.root_group = root_group;
    return node;
}

node_t *
node_new_group(char *name, nodelist_t *members)
{
    node_t *node;
    assert(node = malloc(sizeof *node));
    node->type = NODE_GROUP;
    node->id = -1;
    node->u.group.name = name;
    node->u.group.members = nodelist_reverse(members);
    return node;
}

node_t *
node_new_datatype(char *name)
{
    node_t *node;
    assert(node = malloc(sizeof *node));
    node->type = NODE_DATATYPE;
    node->id = -1;
    node->u.datatype.name = name;
    return node;
}

node_t *
node_new_dataspace(char *type)
{
    node_t *node;
    assert(node = malloc(sizeof *node));
    node->type = NODE_DATASPACE;
    node->id = -1;
    node->u.dataspace.type = type;
    return node;
}

node_t *
node_new_dataset(char *name, nodelist_t *info)
{
    node_t *node;
    assert(node = malloc(sizeof *node));
    node->type = NODE_DATASET;
    node->id = -1;
    node->u.dataset.name = name;
    node->u.dataset.info = info; /* no need to reverse */
    return node;
}

void
node_free(node_t *node)
{
    if (!node) return;
    switch (node->type) {
        case NODE_FILE:
            free(node->u.file.name);
            node_free(node->u.file.root_group);
            break;

        case NODE_GROUP:
            free(node->u.group.name);
            nodelist_free(node->u.group.members);
            break;

        case NODE_DATATYPE:
            free(node->u.datatype.name);
            break;

        case NODE_DATASPACE:
            free(node->u.dataspace.type);
            break;

        case NODE_DATASET:
            free(node->u.dataset.name);
            nodelist_free(node->u.dataset.info);
            break;

        default:
            log_error("cannot free a node of type %d", node->type);
            assert(0);
    }
    free(node);
}

static int
node_create_file(node_t *node, node_t *parent, opt_t *options)
{
    herr_t err;
    const char *name;

    assert(node);
    assert(node->type == NODE_FILE);
    assert(options);
    name = options->output;
    if (name && strcmp(name, node->u.file.name) != 0) {
        log_info("output file name (%s) different from DDL (%s); ignoring DDL file name", name, node->u.file.name);
    }
    if (!name) name = node->u.file.name;

    node->id = H5Fcreate(name, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    if (node->id < 0) {
        err = -1;
        goto fail;
    }
    err = node_create(node->u.file.root_group, node, options);
    if (err < 0) goto fail;
    err = H5Fclose(node->id);
    if (err < 0) goto fail;
    return 0;

fail:
    log_error("failure creating HDF5 file (err = %d)", err);
    if (node->id >= 0) H5Fclose(node->id); /* attempt emergency close */
    return -1;
}

static int
node_create_group(node_t *node, node_t *parent, opt_t *options)
{
    herr_t err;

    assert(node);
    assert(node->type == NODE_GROUP);
    if (parent->type == NODE_FILE && strcmp(node->u.group.name, "/") == 0) {
        /* root group "/" is created automatically */
        node->id = H5Gopen(parent->id, node->u.group.name, H5P_DEFAULT);
    }
    else {
        node->id = H5Gcreate(parent->id, node->u.group.name, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    }
    if (node->id < 0) return -1;
    nodelist_t *p = node->u.group.members;
    while (p) {
        err = node_create(p->node, node, options);
        if (err < 0) return err;
        p = p->next;
    }
    err = H5Gclose(node->id);
    if (err < 0) return err;
    return 0;
}

static hid_t
datatype_id_from_string(const char *name)
{
    struct map_t {
        struct map_t *next;
        const char   *name;
        hid_t         dtype_id;
    };
    static struct map_t *table = NULL;

    assert(name);
    if (!table) {
#define NODE_C_TABLE_ENTRY(type) do { \
    struct map_t *elem; \
    assert(elem = malloc(sizeof *elem)); \
    elem->name = #type; \
    elem->dtype_id = type; \
    SGLIB_LIST_ADD(struct map_t, table, elem, next); \
} while (0)
        NODE_C_TABLE_ENTRY(H5T_IEEE_F32BE);
        NODE_C_TABLE_ENTRY(H5T_IEEE_F32LE);
        NODE_C_TABLE_ENTRY(H5T_IEEE_F64BE);
        NODE_C_TABLE_ENTRY(H5T_IEEE_F64LE);
        NODE_C_TABLE_ENTRY(H5T_STD_I8BE);
        NODE_C_TABLE_ENTRY(H5T_STD_I8LE);
        NODE_C_TABLE_ENTRY(H5T_STD_I16BE);
        NODE_C_TABLE_ENTRY(H5T_STD_I16LE);
        NODE_C_TABLE_ENTRY(H5T_STD_I32BE);
        NODE_C_TABLE_ENTRY(H5T_STD_I32LE);
        NODE_C_TABLE_ENTRY(H5T_STD_I64BE);
        NODE_C_TABLE_ENTRY(H5T_STD_I64LE);
        NODE_C_TABLE_ENTRY(H5T_STD_U8BE);
        NODE_C_TABLE_ENTRY(H5T_STD_U8LE);
        NODE_C_TABLE_ENTRY(H5T_STD_U16BE);
        NODE_C_TABLE_ENTRY(H5T_STD_U16LE);
        NODE_C_TABLE_ENTRY(H5T_STD_U32BE);
        NODE_C_TABLE_ENTRY(H5T_STD_U32LE);
        NODE_C_TABLE_ENTRY(H5T_STD_U64BE);
        NODE_C_TABLE_ENTRY(H5T_STD_U64LE);
        NODE_C_TABLE_ENTRY(H5T_STD_B8BE);
        NODE_C_TABLE_ENTRY(H5T_STD_B8LE);
        NODE_C_TABLE_ENTRY(H5T_STD_B16BE);
        NODE_C_TABLE_ENTRY(H5T_STD_B16LE);
        NODE_C_TABLE_ENTRY(H5T_STD_B32BE);
        NODE_C_TABLE_ENTRY(H5T_STD_B32LE);
        NODE_C_TABLE_ENTRY(H5T_STD_B64BE);
        NODE_C_TABLE_ENTRY(H5T_STD_B64LE);
        NODE_C_TABLE_ENTRY(H5T_NATIVE_CHAR);
        NODE_C_TABLE_ENTRY(H5T_NATIVE_SCHAR);
        NODE_C_TABLE_ENTRY(H5T_NATIVE_UCHAR);
        NODE_C_TABLE_ENTRY(H5T_NATIVE_SHORT);
        NODE_C_TABLE_ENTRY(H5T_NATIVE_USHORT);
        NODE_C_TABLE_ENTRY(H5T_NATIVE_INT);
        NODE_C_TABLE_ENTRY(H5T_NATIVE_UINT);
        NODE_C_TABLE_ENTRY(H5T_NATIVE_LONG);
        NODE_C_TABLE_ENTRY(H5T_NATIVE_ULONG);
        NODE_C_TABLE_ENTRY(H5T_NATIVE_LLONG);
        NODE_C_TABLE_ENTRY(H5T_NATIVE_ULLONG);
        NODE_C_TABLE_ENTRY(H5T_NATIVE_FLOAT);
        NODE_C_TABLE_ENTRY(H5T_NATIVE_DOUBLE);
        NODE_C_TABLE_ENTRY(H5T_NATIVE_LDOUBLE);
        NODE_C_TABLE_ENTRY(H5T_NATIVE_B8);
        NODE_C_TABLE_ENTRY(H5T_NATIVE_B16);
        NODE_C_TABLE_ENTRY(H5T_NATIVE_B32);
        NODE_C_TABLE_ENTRY(H5T_NATIVE_B64);
        NODE_C_TABLE_ENTRY(H5T_NATIVE_OPAQUE);
        NODE_C_TABLE_ENTRY(H5T_NATIVE_HADDR);
        NODE_C_TABLE_ENTRY(H5T_NATIVE_HSIZE);
        NODE_C_TABLE_ENTRY(H5T_NATIVE_HSSIZE);
        NODE_C_TABLE_ENTRY(H5T_NATIVE_HERR);
        NODE_C_TABLE_ENTRY(H5T_NATIVE_HBOOL);
#undef NODE_C_TABLE_ENTRY
    }
    struct map_t *result;
#define NODE_C_TABLE_NAME_COMPARATOR(x, y) strcmp((x)->name, (y))
    SGLIB_LIST_FIND_MEMBER(struct map_t, table, name, NODE_C_TABLE_NAME_COMPARATOR, next, result);
#undef NODE_C_TABLE_NAME_COMPARATOR
    return result ? result->dtype_id : -1;
}

static int
node_create_datatype(node_t *node, node_t *parent, opt_t *options)
{
    hid_t dtype_id;

    assert(node);
    assert(node->type == NODE_DATATYPE);
    dtype_id = datatype_id_from_string(node->u.datatype.name);
    if (dtype_id < 0) {
        log_error("cannot convert type '%s' to ID", node->u.datatype.name);
        return -1;
    }
    node->id = H5Tcopy(dtype_id);
    if (node->id < 0) return -1;
    /* cannot close datatype before dataset is written */
    return 0;
}

static int
node_create_dataspace(node_t *node, node_t *parent, opt_t *options)
{
    assert(node);
    assert(node->type == NODE_DATASPACE);
    assert(strcmp(node->u.dataspace.type, "SCALAR") == 0);
    node->id = H5Screate(H5S_SCALAR);
    if (node->id < 0) return -1;
    /* cannot close dataspace before dataset is written */
    return 0;
}

static int
node_create_dataset(node_t *node, node_t *parent, opt_t *options)
{
    node_type_t type;
    nodelist_t *p_datatype, *p_dataspace;
    node_t *datatype, *dataspace;
    herr_t err;

    assert(node);
    assert(node->type == NODE_DATASET);
    /* TODO this stuff should be offloaded to node_new_dataset() ! */
    type = NODE_DATATYPE;
    if (!(p_datatype = nodelist_find(node->u.dataset.info, nodelist_find_node_by_type, &type))) {
        log_error("dataset %s does not have a datatype", node->u.dataset.name);
        return -1;
    }
    if (nodelist_find(p_datatype->next, nodelist_find_node_by_type, &type)) {
        log_error("dataset %s has more than one datatype", node->u.dataset.name);
        return -1;
    }
    datatype = p_datatype->node;
    type = NODE_DATASPACE;
    if (!(p_dataspace = nodelist_find(node->u.dataset.info, nodelist_find_node_by_type, &type))) {
        log_error("dataset %s does not have a dataspace", node->u.dataset.name);
        return -1;
    }
    if (nodelist_find(p_dataspace->next, nodelist_find_node_by_type, &type)) {
        log_error("dataset %s has more than one dataspace", node->u.dataset.name);
        return -1;
    }
    dataspace = p_dataspace->node;
    /* TODO end offload */
    err = node_create_datatype(datatype, node, options);
    if (err < 0) return err;
    err = node_create_dataspace(dataspace, node, options);
    if (err < 0) return err;
    node->id = H5Dcreate(parent->id, node->u.dataset.name, datatype->id, dataspace->id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    if (node->id < 0) return -1;
    /* TODO write data! */
    err = H5Dclose(node->id);
    if (err < 0) return err;
    err = H5Sclose(dataspace->id);
    if (err < 0) return err;
    err = H5Tclose(datatype->id);
    if (err < 0) return err;
    return 0;
}

int
node_create(node_t *node, node_t *parent, opt_t *options)
{
    assert(node);
    switch (node->type) {
        case NODE_FILE:
            return node_create_file(node, parent, options);

        case NODE_GROUP:
            return node_create_group(node, parent, options);

        case NODE_DATATYPE:
            return node_create_datatype(node, parent, options);

        case NODE_DATASPACE:
            return node_create_dataspace(node, parent, options);

        case NODE_DATASET:
            return node_create_dataset(node, parent, options);

        default:
            log_error("cannot write a node of type %d", node->type);
            assert(0);
    }
}

nodelist_t *
nodelist_prepend(nodelist_t *list, node_t *node)
{
    nodelist_t *el;
    assert(node);
    assert(el = malloc(sizeof *el));
    el->next = NULL;
    el->node = node;
    SGLIB_LIST_ADD(nodelist_t, list, el, next);
    return list;
}

void
nodelist_free(nodelist_t *list)
{
    if (!list) return;
    SGLIB_LIST_MAP_ON_ELEMENTS(nodelist_t, list, p, next, (node_free(p->node), free(p)));
}

nodelist_t *
nodelist_reverse(nodelist_t *list)
{
    if (!list) return NULL;
    SGLIB_LIST_REVERSE(nodelist_t, list, next);
    return list;
}

nodelist_t *
nodelist_find(nodelist_t *list, nodelist_find_t *func, void *userdata)
{
    nodelist_t *el;
    if (!list) return NULL;
    SGLIB_LIST_FIND_MEMBER(nodelist_t, list, userdata, func, next, el);
    return el;
}

int
nodelist_find_node_by_type(nodelist_t *el, void *userdata)
{
    node_t *node = el->node;
    node_type_t type = *(node_type_t *) userdata;
    return !(node->type == type); /* have to return 0 for equality */
}
