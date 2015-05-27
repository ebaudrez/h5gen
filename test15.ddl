## tap_testname = dataset creation - simple dataspace with floating-point literals in value list
HDF5 "test15.h5" {
GROUP "/" {
    DATASET "def" {
        DATATYPE H5T_IEEE_F32LE
        DATASPACE SIMPLE { ( 3 ) / ( 3 ) }
        DATA {
            (0): -7., 2.99, -3.14
        }
    }
}
}
