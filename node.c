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

        default:
            log_error("unknown node type %d", node->type);
            assert(0);
    }
    free(node);
}

/* forward declaration */
static int node_create(node_t *node, node_t *parent);

static int
node_create_group(node_t *node, node_t *parent)
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
        err = node_create(p->node, node);
        if (err < 0) return err;
        p = p->next;
    }
    err = H5Gclose(node->id);
    if (err < 0) return err;
    return 0;
}

static int
node_create(node_t *node, node_t *parent)
{
    assert(node);
    switch (node->type) {
        case NODE_GROUP:
            return node_create_group(node, parent);

        default:
            log_error("cannot write a node of type %d", node->type);
            assert(0);
    }
}

int
node_create_file(node_t *node, opt_t *options)
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
    err = node_create(node->u.file.root_group, node);
    if (err < 0) goto fail;
    err = H5Fclose(node->id);
    if (err < 0) goto fail;
    return 0;

fail:
    log_error("failure creating HDF5 file (err = %d)", err);
    if (node->id >= 0) H5Fclose(node->id); /* attempt emergency close */
    return -1;
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
