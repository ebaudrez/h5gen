## tap_testname = nested groups and datasets (test file from h5tonc)
HDF5 "test18.h5" {
    GROUP "/" {
        DATASET "First" {
            DATATYPE H5T_STD_U32LE
            DATASPACE SCALAR
            DATA {
                (0): 1
            }
        }
        GROUP "my subgroup" {
            DATASET "squarish" {
                DATATYPE H5T_STD_U32LE
                DATASPACE SIMPLE { (10, 10) / (10, 10) }
                DATA {
                    (0,0): 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                    (1,0): 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                    (2,0): 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                    (3,0): 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                    (4,0): 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                    (5,0): 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                    (6,0): 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                    (7,0): 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                    (8,0): 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                    (9,0): 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
                }
            }
            DATASET "testme" {
                DATATYPE H5T_STD_I16BE
                DATASPACE SIMPLE { (3, 5) / (3, 5) }
                DATA {
                    (0,0): -19, -20, -21, -22, -23,
                    (1,0):   1,   2,   3,   4,   5,
                    (2,0):   0,   3,   6,   9, -12
                }
            }
        }
        DATASET "pico SD" {
            DATATYPE H5T_STD_U8LE
            DATASPACE SIMPLE { (3) / (3) }
            DATA {
                (0): 3, 6, 9
            }
        }
    }
}
