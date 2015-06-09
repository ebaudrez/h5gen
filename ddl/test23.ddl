## tap_testname = datatype creation - fixed-length string datatype (scalar attribute)
HDF5 "test23.h5" {
GROUP "/" {
   ATTRIBUTE "File Creation Time" {
      DATATYPE  H5T_STRING {
            STRSIZE 18;
            STRPAD H5T_STR_NULLTERM;
            CSET H5T_CSET_ASCII;
            CTYPE H5T_C_S1;
         }
      DATASPACE  SCALAR
      DATA {
      (0): "20070319 18:13:46"
      }
   }
   ATTRIBUTE "File Name" {
      DATATYPE  H5T_STRING {
            STRSIZE 47;
            STRPAD H5T_STR_NULLTERM;
            CSET H5T_CSET_ASCII;
            CTYPE H5T_C_S1;
         }
      DATASPACE  SCALAR
      DATA {
      (0): "G2_SEV1_L20_HR_SOL_TH_20040701_120000_V003.hdf"
      }
   }
}
}
