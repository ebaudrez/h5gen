## tap_testname = dataset creation - scalar dataspace, integral datatype
## h5dump_opt = -y
HDF5 "test08.h5" {
GROUP "/" {
    DATASET "dset" {
        DATATYPE H5T_STD_U32LE
        DATASPACE SCALAR
        DATA {
           0
        }
    }
}
}
