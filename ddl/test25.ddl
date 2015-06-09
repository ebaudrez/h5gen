## tap_testname = datatype creation - fixed-length string datatype (array)
HDF5 "test25.h5" {
GROUP "/" {
   DATASET "Test" {
      DATATYPE  H5T_STRING {
            STRSIZE 15;
            STRPAD H5T_STR_NULLTERM;
            CSET H5T_CSET_ASCII;
            CTYPE H5T_C_S1;
         }
      DATASPACE  SIMPLE { ( 4 ) / ( 4 ) }
      DATA {
      (0): "this", "is", "a", "text"
      }
   }
}
}
