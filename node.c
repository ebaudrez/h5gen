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
node_new_group(const char *name)
{
    node_t *node;
    assert(node = malloc(sizeof *node));
    node->type = NODE_GROUP;
    node->u.group.name = strdup(name);
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
            break;

        default:
            log_error("unknown node type %d", node->type);
            assert(0);
    }
}

int
node_write_file(node_t *node, const char *name)
{
    hid_t file;
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

    err = H5Fclose(file);
    if (err < 0) goto fail;
    return 0;

fail:
    log_error("failure creating HDF5 file (err = %d)", err);
    if (file >= 0) H5Fclose(file); /* attempt emergency close */
    return -1;
}
