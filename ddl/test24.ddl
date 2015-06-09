## tap_testname = datatype creation - fixed-length string datatype (array, without data values)
## h5dump_opt = -H
HDF5 "test24.h5" {
GROUP "/" {
   GROUP "Times" {
      DATASET "Time (per row)" {
         DATATYPE  H5T_STRING {
               STRSIZE 22;
               STRPAD H5T_STR_NULLTERM;
               CSET H5T_CSET_ASCII;
               CTYPE H5T_C_S1;
            }
         DATASPACE  SIMPLE { ( 1237 ) / ( 1237 ) }
      }
   }
}
}
