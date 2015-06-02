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
node_new_attribute(char *name, nodelist_t *info)
{
    node_t *node;
    assert(node = malloc(sizeof *node));
    node->type = NODE_ATTRIBUTE;
    node->id = -1;
    node->u.attribute.name = name;
    if (!(node->u.attribute.datatype = nodelist_extract_unique_node_by_type(&info, NODE_DATATYPE))) {
        log_error("cannot extract datatype from attribute %s", node->u.attribute.name);
    }
    if (!(node->u.attribute.dataspace = nodelist_extract_unique_node_by_type(&info, NODE_DATASPACE))) {
        log_error("cannot extract dataspace from attribute %s", node->u.attribute.name);
    }
    /* data is optional! */
    if (!(node->u.attribute.data = nodelist_extract_unique_node_by_type(&info, NODE_DATA))) {
        log_warn("cannot extract data from attribute %s", node->u.attribute.name);
    }
    if (nodelist_length(info) > 0) {
        log_warn("%d unrecognized elements in attribute", nodelist_length(info));
        nodelist_free(info);
    }
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
    node->u.data.mem_type_id = -1;
    node->u.data.buf = NULL;
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
    if (!(node->u.dataset.datatype = nodelist_extract_unique_node_by_type(&info, NODE_DATATYPE))) {
        log_error("cannot extract datatype from dataset %s", node->u.dataset.name);
    }
    if (!(node->u.dataset.dataspace = nodelist_extract_unique_node_by_type(&info, NODE_DATASPACE))) {
        log_error("cannot extract dataspace from dataset %s", node->u.dataset.name);
    }
    /* attributes are optional */
    node->u.dataset.attributes = nodelist_extract_by_type(&info, NODE_ATTRIBUTE);
    /* data is optional! */
    if (!(node->u.dataset.data = nodelist_extract_unique_node_by_type(&info, NODE_DATA))) {
        log_warn("cannot extract data from dataset %s", node->u.dataset.name);
    }
    if (nodelist_length(info) > 0) {
        log_warn("%d unrecognized elements in dataset", nodelist_length(info));
        nodelist_free(info);
    }
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
node_new_datatype_float(hid_t id)
{
    node_t *node;
    assert(node = malloc(sizeof *node));
    node->type = NODE_DATATYPE;
    node->id = -1;
    node->u.datatype.class = H5T_FLOAT;
    node->u.datatype.template = id;
    node->u.datatype.size = 0;
    node->u.datatype.strpad = H5T_PAD_ERROR;
    node->u.datatype.cset = H5T_CSET_ERROR;
    return node;
}

node_t *
node_new_datatype_integer(hid_t id)
{
    node_t *node;
    assert(node = malloc(sizeof *node));
    node->type = NODE_DATATYPE;
    node->id = -1;
    node->u.datatype.class = H5T_INTEGER;
    node->u.datatype.template = id;
    node->u.datatype.size = 0;
    node->u.datatype.strpad = H5T_PAD_ERROR;
    node->u.datatype.cset = H5T_CSET_ERROR;
    return node;
}

node_t *
node_new_datatype_string(size_t strsize, H5T_str_t strpad, H5T_cset_t cset, hid_t ctype)
{
    node_t *node;
    assert(node = malloc(sizeof *node));
    node->type = NODE_DATATYPE;
    node->id = -1;
    node->u.datatype.class = H5T_STRING;
    node->u.datatype.template = ctype;
    node->u.datatype.size = strsize;
    node->u.datatype.strpad = strpad;
    node->u.datatype.cset = cset;
    return node;
}

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

node_t *
node_new_string(char *value)
{
    node_t *node;
    assert(node = malloc(sizeof *node));
    node->type = NODE_STRING;
    node->id = -1;
    node->u.string.value = value;
    return node;
}

void
node_free(node_t *node)
{
    if (!node) return;
    switch (node->type) {
        case NODE_ATTRIBUTE:
            free(node->u.attribute.name);
            node_free(node->u.attribute.datatype);
            node_free(node->u.attribute.dataspace);
            node_free(node->u.attribute.data);
            break;

        case NODE_DATA:
            nodelist_free(node->u.data.values);
            free(node->u.data.buf);
            break;

        case NODE_DATASET:
            free(node->u.dataset.name);
            node_free(node->u.dataset.datatype);
            node_free(node->u.dataset.dataspace);
            nodelist_free(node->u.dataset.attributes);
            node_free(node->u.dataset.data);
            break;

        case NODE_DATASPACE:
            free(node->u.dataspace.cur_dims);
            free(node->u.dataspace.max_dims);
            break;

        case NODE_DATATYPE:
            break;

        case NODE_FILE:
            free(node->u.file.name);
            node_free(node->u.file.root_group);
            break;

        case NODE_GROUP:
            free(node->u.group.name);
            nodelist_free(node->u.group.members);
            break;

        case NODE_INTEGER:
            break;

        case NODE_REALNUM:
            break;

        case NODE_STRING:
            free(node->u.string.value);
            break;

        default:
            log_error("cannot free a node of type %d", node->type);
            assert(0);
    }
    free(node);
}

static int
node_create_attribute(node_t *node, node_t *parent, opt_t *options)
{
    herr_t err;
    assert(node);
    assert(node->type == NODE_ATTRIBUTE);
    err = node_create(node->u.attribute.datatype, node, options);
    if (err < 0) return err;
    err = node_create(node->u.attribute.dataspace, node, options);
    if (err < 0) return err;
    node->id = H5Acreate(parent->id, node->u.attribute.name, node->u.attribute.datatype->id,
            node->u.attribute.dataspace->id, H5P_DEFAULT, H5P_DEFAULT);
    if (node->id < 0) return -1;
    if (node->u.attribute.data) {
        err = node_create(node->u.attribute.data, node, options);
        if (err < 0) return err;
        err = H5Awrite(node->id, node->u.attribute.data->u.data.mem_type_id, node->u.attribute.data->u.data.buf);
        if (err < 0) return err;
        err = H5Tclose(node->u.attribute.data->u.data.mem_type_id);
        if (err < 0) return err;
    }
    err = H5Aclose(node->id);
    if (err < 0) return err;
    err = H5Sclose(node->u.attribute.dataspace->id);
    if (err < 0) return err;
    err = H5Tclose(node->u.attribute.datatype->id);
    if (err < 0) return err;
    return 0;
}

#define COPY_NODELIST_TO_NUMERIC_BUF(src_, dst_, dtype, n) do { \
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
#define COPY_NODELIST_TO_CHARACTER_BUF(src_, dst_, n, sz) do { \
    nodelist_t *src = (src_); \
    char *dst = (dst_), *start = (dst_); \
    while (src) { \
        assert(dst - start < ((n) * (sz))); /* guard against buffer overrun */ \
        switch (src->node->type) { \
            /* in principle, could convert from numeric with snprintf() */ \
            case NODE_STRING: \
                strncpy(dst, src->node->u.string.value, sz); \
                dst += sz; \
                break; \
            default: log_error("cannot copy data value from node of type %d", src->node->type); break; \
        } \
        src = src->next; \
    } \
    assert(dst - start == ((n) * (sz))); /* check that all values were supplied */ \
} while (0)
static int
node_create_data(node_t *node, node_t *parent, opt_t *options)
{
    node_t *dataspace, *datatype;
    hsize_t n;
    size_t sz;
    herr_t err;

    assert(node);
    assert(node->type == NODE_DATA);
    switch (parent->type) {
        case NODE_ATTRIBUTE:
            assert(dataspace = parent->u.attribute.dataspace);
            assert(datatype = parent->u.attribute.datatype);
            break;

        case NODE_DATASET:
            assert(dataspace = parent->u.dataset.dataspace);
            assert(datatype = parent->u.dataset.datatype);
            break;

        default:
            log_error("a DATA block must belong to either an ATTRIBUTE or a DATASET");
            return -1;
    }
    assert(dataspace->type == NODE_DATASPACE);
    assert(datatype->type == NODE_DATATYPE);
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
            log_error("cannot create data for a dataspace of type %d", dataspace->u.dataspace.type);
            return -1;
    }
    switch (datatype->u.datatype.class) {
        case H5T_INTEGER:
            node->u.data.mem_type_id = H5Tcopy(H5T_NATIVE_INT);
            if (node->u.data.mem_type_id < 0) return -1;
            break;

        case H5T_FLOAT:
            node->u.data.mem_type_id = H5Tcopy(H5T_NATIVE_DOUBLE);
            if (node->u.data.mem_type_id < 0) return -1;
            break;

        case H5T_STRING:
            node->u.data.mem_type_id = H5Tcopy(H5T_C_S1);
            if (node->u.data.mem_type_id < 0) return -1;
            err = H5Tset_size(node->u.data.mem_type_id, datatype->u.datatype.size);
            if (err < 0) return err;
            err = H5Tset_strpad(node->u.data.mem_type_id, H5T_STR_NULLTERM);
            if (err < 0) return err;
            break;

        default:
            log_error("cannot create a memory datatype for datatype class %d", datatype->u.datatype.class);
            return -1;
    }
    sz = H5Tget_size(node->u.data.mem_type_id);
    assert(n * sz > 0);
    assert(node->u.data.buf = malloc(n * sz));
    switch (datatype->u.datatype.class) {
        case H5T_INTEGER: COPY_NODELIST_TO_NUMERIC_BUF(node->u.data.values, node->u.data.buf, int, n); break;
        case H5T_FLOAT: COPY_NODELIST_TO_NUMERIC_BUF(node->u.data.values, node->u.data.buf, double, n); break;
        case H5T_STRING: COPY_NODELIST_TO_CHARACTER_BUF(node->u.data.values, node->u.data.buf, n, sz); break;
        /* datatype class is known to be good, from the switch statement above */
    }
    return 0;
}
#undef COPY_NODELIST_TO_NUMERIC_BUF
#undef COPY_NODELIST_TO_CHARACTER_BUF

static int
node_create_dataset(node_t *node, node_t *parent, opt_t *options)
{
    herr_t err;
    assert(node);
    assert(node->type == NODE_DATASET);
    err = node_create(node->u.dataset.datatype, node, options);
    if (err < 0) return err;
    err = node_create(node->u.dataset.dataspace, node, options);
    if (err < 0) return err;
    node->id = H5Dcreate(parent->id, node->u.dataset.name, node->u.dataset.datatype->id,
            node->u.dataset.dataspace->id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    if (node->id < 0) return -1;
    if (node->u.dataset.attributes) {
        nodelist_t *p = node->u.dataset.attributes;
        while (p) {
            err = node_create(p->node, node, options);
            if (err < 0) return err;
            p = p->next;
        }
    }
    if (node->u.dataset.data) {
        err = node_create(node->u.dataset.data, node, options);
        if (err < 0) return err;
        err = H5Dwrite(node->id, node->u.dataset.data->u.data.mem_type_id, H5S_ALL, H5S_ALL, H5P_DEFAULT, node->u.dataset.data->u.data.buf);
        if (err < 0) return err;
        err = H5Tclose(node->u.dataset.data->u.data.mem_type_id);
        if (err < 0) return err;
    }
    err = H5Dclose(node->id);
    if (err < 0) return err;
    err = H5Sclose(node->u.dataset.dataspace->id);
    if (err < 0) return err;
    err = H5Tclose(node->u.dataset.datatype->id);
    if (err < 0) return err;
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

static int
node_create_datatype(node_t *node, node_t *parent, opt_t *options)
{
    assert(node);
    assert(node->type == NODE_DATATYPE);
    switch (node->u.datatype.class) {
        case H5T_INTEGER:
        case H5T_FLOAT:
        case H5T_STRING:
            node->id = H5Tcopy(node->u.datatype.template);
            break;

        default:
            log_error("cannot create a datatype of type class %d", node->u.datatype.class);
            return -1;
    }
    if (node->id < 0) return -1;
    if (node->u.datatype.class == H5T_STRING) {
        herr_t err;
        err = H5Tset_size(node->id, node->u.datatype.size);
        if (err < 0) return err;
        err = H5Tset_strpad(node->id, node->u.datatype.strpad);
        if (err < 0) return err;
        err = H5Tset_cset(node->id, node->u.datatype.cset);
        if (err < 0) return err;
    }
    /* cannot close datatype before dataset is written */
    return 0;
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

int
node_create(node_t *node, node_t *parent, opt_t *options)
{
    assert(node);
    switch (node->type) {
        case NODE_ATTRIBUTE:
            return node_create_attribute(node, parent, options);

        case NODE_DATA:
            return node_create_data(node, parent, options);

        case NODE_DATASET:
            return node_create_dataset(node, parent, options);

        case NODE_DATASPACE:
            return node_create_dataspace(node, parent, options);

        case NODE_DATATYPE:
            return node_create_datatype(node, parent, options);

        case NODE_FILE:
            return node_create_file(node, parent, options);

        case NODE_GROUP:
            return node_create_group(node, parent, options);

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
nodelist_extract_by_type(nodelist_t **list, node_type_t type)
{
    nodelist_t *out = NULL, *p;
    while (1) {
        DL_SEARCH_SCALAR(*list, p, node->type, type);
        if (!p) break;
        DL_DELETE(*list, p);
        DL_APPEND(out, p);
    }
    return out;
}

node_t *
nodelist_extract_unique_node_by_type(nodelist_t **list, node_type_t type)
{
    nodelist_t *tmp;
    if (!(tmp = nodelist_extract_by_type(list, type))) return NULL;
    if (nodelist_length(tmp) > 1) {
        log_error("list contains more than one node of type %d", type);
        return NULL;
    }
    return tmp->node;
}
