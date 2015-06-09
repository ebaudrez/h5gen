## tap_testname = dataset creation - scalar dataspace, integral datatype, data != 0
## h5dump_opt = -y
HDF5 "test09.h5" {
GROUP "/" {
    DATASET "dset" {
        DATATYPE H5T_STD_U32LE
        DATASPACE SCALAR
        DATA {
           981
        }
    }
}
}
