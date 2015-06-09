## tap_testname = dataset creation - scalar dataspace, floating-point datatype
HDF5 "test11.h5" {
GROUP "/" {
    DATASET "dset" {
        DATATYPE H5T_IEEE_F64BE
        DATASPACE SCALAR
        DATA {
        (0): 3.14
        }
    }
}
}
