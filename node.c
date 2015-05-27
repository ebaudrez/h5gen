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
#include "utlist.h"

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
    node->u.group.members = members;
    return node;
}

node_t *
node_new_datatype(hid_t id)
{
    node_t *node;
    assert(node = malloc(sizeof *node));
    node->type = NODE_DATATYPE;
    node->id = -1;
    node->u.datatype.id = id;
    return node;
}

node_t *
node_new_dataspace_scalar(void)
{
    node_t *node;
    assert(node = malloc(sizeof *node));
    node->type = NODE_DATASPACE;
    node->id = -1;
    node->u.dataspace.type = DATASPACE_SCALAR;
    node->u.dataspace.rank = 0;
    node->u.dataspace.cur_dims = NULL;
    node->u.dataspace.max_dims = NULL;
    return node;
}

node_t *
node_new_dataspace_simple(nodelist_t *cur_dims, nodelist_t *max_dims)
{
    node_t *node;
    int rank;
    nodelist_t *src;
    hsize_t *dst;

    assert(cur_dims);
    assert(max_dims);
    assert(node = malloc(sizeof *node));
    node->type = NODE_DATASPACE;
    node->id = -1;
    node->u.dataspace.type = DATASPACE_SIMPLE;
    rank = nodelist_length(cur_dims);
    assert(nodelist_length(max_dims) == rank);
    node->u.dataspace.rank = rank;
    /* copy cur_dims */
    src = cur_dims;
    assert(dst = node->u.dataspace.cur_dims = malloc(rank * sizeof(hsize_t)));
    while (src) {
        assert(src->node->type == NODE_INTEGER);
        *dst++ = src->node->u.integer.value;
        src = src->next;
    }
    nodelist_free(cur_dims);
    /* copy max_dims */
    src = max_dims;
    assert(dst = node->u.dataspace.max_dims = malloc(rank * sizeof(hsize_t)));
    while (src) {
        assert(src->node->type == NODE_INTEGER);
        *dst++ = src->node->u.integer.value;
        src = src->next;
    }
    nodelist_free(max_dims);
    return node;
}

node_t *
node_new_data(nodelist_t *values)
{
    node_t *node;
    assert(node = malloc(sizeof *node));
    node->type = NODE_DATA;
    node->id = -1;
    node->u.data.values = values;
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
    node->u.dataset.info = info;
    return node;
}

node_t *
node_new_integer(int value)
{
    node_t *node;
    assert(node = malloc(sizeof *node));
    node->type = NODE_INTEGER;
    node->id = -1;
    node->u.integer.value = value;
    return node;
}

node_t *
node_new_realnum(double value)
{
    node_t *node;
    assert(node = malloc(sizeof *node));
    node->type = NODE_REALNUM;
    node->id = -1;
    node->u.realnum.value = value;
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
            break;

        case NODE_DATASPACE:
            free(node->u.dataspace.cur_dims);
            free(node->u.dataspace.max_dims);
            break;

        case NODE_DATA:
            nodelist_free(node->u.data.values);
            break;

        case NODE_DATASET:
            free(node->u.dataset.name);
            nodelist_free(node->u.dataset.info);
            break;

        case NODE_INTEGER:
            break;

        case NODE_REALNUM:
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

static int
node_create_datatype(node_t *node, node_t *parent, opt_t *options)
{
    assert(node);
    assert(node->type == NODE_DATATYPE);
    node->id = H5Tcopy(node->u.datatype.id);
    if (node->id < 0) return -1;
    /* cannot close datatype before dataset is written */
    return 0;
}

static int
node_create_dataspace(node_t *node, node_t *parent, opt_t *options)
{
    assert(node);
    assert(node->type == NODE_DATASPACE);
    switch (node->u.dataspace.type) {
        case DATASPACE_SCALAR:
            node->id = H5Screate(H5S_SCALAR);
            break;

        case DATASPACE_SIMPLE:
            node->id = H5Screate_simple(node->u.dataspace.rank,
                    node->u.dataspace.cur_dims, node->u.dataspace.max_dims);
            break;

        default:
            log_error("cannot create a dataspace of type %d", node->u.dataspace.type);
            return -1;
    }
    if (node->id < 0) return -1;
    /* cannot close dataspace before dataset is written */
    return 0;
}

#define COPY_DATA_FROM_NODELIST(src_, dst_, dtype, n) do { \
    nodelist_t *src = (src_); \
    dtype *dst = (dst_), *start = (dst_); \
    while (src) { \
        assert(dst - start < (n)); /* guard against buffer overrun */ \
        switch (src->node->type) { \
            case NODE_INTEGER: *dst++ = src->node->u.integer.value; break; \
            case NODE_REALNUM: *dst++ = src->node->u.realnum.value; break; \
            default: log_error("cannot copy data value from node of type %d", src->node->type); break; \
        } \
        src = src->next; \
    } \
    assert(dst - start == (n)); /* check that all values were supplied */ \
} while (0)
static void *
prepare_data(node_t *datatype, node_t *dataspace, node_t *data, hid_t *mem_type_id, opt_t *options)
{
    H5T_class_t class;
    hsize_t n;
    size_t sz;
    void *buf;
    assert(datatype);
    assert(dataspace);
    assert(data);
    assert(mem_type_id);
    switch (dataspace->u.dataspace.type) {
        case DATASPACE_SCALAR:
            n = 1;
            break;

        case DATASPACE_SIMPLE:
            n = 1;
            {
                int r, rank = dataspace->u.dataspace.rank;
                for (r = 0; r < rank; r++) {
                    n *= dataspace->u.dataspace.cur_dims[r];
                }
            }
            break;

        default:
            log_error("cannot prepare data for a dataspace of type %d", dataspace->u.dataspace.type);
            return NULL;
    }
    class = H5Tget_class(datatype->u.datatype.id);
    switch (class) {
        case H5T_INTEGER: *mem_type_id = H5T_NATIVE_INT; break;
        case H5T_FLOAT: *mem_type_id = H5T_NATIVE_DOUBLE; break;
        default:
            log_error("cannot choose a memory datatype for datatype class %d", class);
            return NULL;
    }
    sz = H5Tget_size(*mem_type_id);
    assert(n * sz > 0);
    assert(buf = malloc(n * sz));
    switch (class) {
        case H5T_INTEGER: COPY_DATA_FROM_NODELIST(data->u.data.values, buf, int, n); break;
        case H5T_FLOAT: COPY_DATA_FROM_NODELIST(data->u.data.values, buf, double, n); break;
        /* 'class' is known to be good, from the switch statement above */
    }
    return buf;
}
#undef COPY_DATA_FROM_NODELIST

static int
node_create_dataset(node_t *node, node_t *parent, opt_t *options)
{
    node_type_t type;
    nodelist_t *p_datatype, *p_dataspace, *p_data;
    node_t *datatype, *dataspace, *data;
    hid_t mem_type_id;
    void *buf;
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
    /* TODO begin offload */
    type = NODE_DATA;
    if (!(p_data = nodelist_find(node->u.dataset.info, nodelist_find_node_by_type, &type))) {
        log_warn("dataset %s does not have data", node->u.dataset.name);
        /* FIXME memory leak because resources not closed properly */
        return 0;
    }
    if (nodelist_find(p_data->next, nodelist_find_node_by_type, &type)) {
        log_error("dataset %s has more than one list of data values", node->u.dataset.name);
        return -1;
    }
    data = p_data->node;
    /* TODO end offload */
    assert(buf = prepare_data(datatype, dataspace, data, &mem_type_id, options));
    err = H5Dwrite(node->id, mem_type_id, H5S_ALL, H5S_ALL, H5P_DEFAULT, buf);
    free(buf);
    if (err < 0) return err;
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
            log_error("cannot create a node of type %d", node->type);
            assert(0);
    }
}

nodelist_t *
nodelist_append(nodelist_t *list, node_t *node)
{
    nodelist_t *el;
    assert(node);
    assert(el = malloc(sizeof *el));
    el->node = node;
    DL_APPEND(list, el);
    return list;
}

void
nodelist_free(nodelist_t *list)
{
    nodelist_t *p, *tmp;
    DL_FOREACH_SAFE(list, p, tmp) {
        node_free(p->node);
        free(p);
    }
}

size_t
nodelist_length(nodelist_t *list)
{
    size_t result;
    nodelist_t *p;
    DL_COUNT(list, p, result);
    return result;
}

nodelist_t *
nodelist_find(nodelist_t *list, nodelist_find_t *func, void *userdata)
{
    nodelist_t *el;
    if (!list) return NULL;
    DL_SEARCH(list, el, userdata, func);
    return el;
}

int
nodelist_find_node_by_type(nodelist_t *el, void *userdata)
{
    node_t *node = el->node;
    node_type_t type = *(node_type_t *) userdata;
    return !(node->type == type); /* have to return 0 for equality */
}
