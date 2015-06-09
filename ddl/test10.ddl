## tap_testname = dataset creation - scalar dataspace, leading index
HDF5 "test10.h5" {
GROUP "/" {
    DATASET "dset" {
        DATATYPE H5T_STD_U32LE
        DATASPACE SCALAR
        DATA {
        (0): 0
        }
    }
}
}
