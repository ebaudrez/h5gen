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
node_new_file(const char *name, node_t *root_group)
{
    node_t *node;
    assert(node = malloc(sizeof *node));
    node->type = NODE_FILE;
    node->u.file.name = strdup(name);
    node->u.file.root_group = root_group;
    return node;
}

node_t *
node_new_group(const char *name, nodelist_t *members)
{
    node_t *node;
    assert(node = malloc(sizeof *node));
    node->type = NODE_GROUP;
    node->u.group.name = strdup(name);
    node->u.group.members = members;
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
}

/* forward declaration */
static int node_create(node_t *node, hid_t locid);

static int
node_create_group(node_t *node, hid_t locid)
{
    hid_t group;
    herr_t err;

    assert(node);
    assert(node->type == NODE_GROUP);
    group = H5Gcreate(locid, node->u.group.name, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    if (group < 0) return -1;
    nodelist_t *p = node->u.group.members;
    while (p) {
        node_create(p->node, group);
        p = p->next;
    }
    err = H5Gclose(group);
    if (err < 0) return err;
    return 0;
}

static int
node_create(node_t *node, hid_t locid)
{
    assert(node);
    switch (node->type) {
        case NODE_GROUP:
            return node_create_group(node, locid);

        default:
            log_error("cannot write a node of type %d", node->type);
            assert(0);
    }
}

int
node_create_file(node_t *node, const char *name)
{
    hid_t file, root;
    herr_t err;

    assert(node);
    assert(node->type == NODE_FILE);
    if (name && strcmp(name, node->u.file.name) != 0) {
        log_info("output file name (%s) different from DDL (%s); ignoring DDL file name", name, node->u.file.name);
    }
    if (!name) name = node->u.file.name;

    file = H5Fcreate(name, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    if (file < 0) {
        err = -1;
        goto fail;
    }

    /* no need to create the root group; it's created automatically */
    node_t *r = node->u.file.root_group;
    assert(r->type == NODE_GROUP);

    /* but must descend into it and iterate over its members */
    root = H5Gopen(file, "/", H5P_DEFAULT);
    nodelist_t *p = r->u.group.members;
    while (p) {
        node_create(p->node, root);
        p = p->next;
    }

    err = H5Gclose(root);
    if (err < 0) goto fail;
    err = H5Fclose(file);
    if (err < 0) goto fail;
    return 0;

fail:
    log_error("failure creating HDF5 file (err = %d)", err);
    if (file >= 0) H5Fclose(file); /* attempt emergency close */
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
    /* TODO */
    assert(0);
}

nodelist_t *
nodelist_reverse(nodelist_t *list)
{
    if (!list) return NULL;
    SGLIB_LIST_REVERSE(nodelist_t, list, next);
    return list;
}
