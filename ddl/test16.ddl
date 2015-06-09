## tap_testname = dataset creation - mixed integer and floating-point literals in value list
HDF5 "test16.h5" {
GROUP "/" {
    DATASET "def" {
        DATATYPE H5T_IEEE_F32LE
        DATASPACE SIMPLE { ( 4 ) / ( 4 ) }
        DATA {
            (0): -7, 2.990, -3.14, 2
        }
    }
}
}
