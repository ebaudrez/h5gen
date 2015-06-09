## tap_testname = group creation - multiple subgroups (intended to be tested for memory leaks)
HDF5 "test05.h5" {
GROUP "/" {
    GROUP "abc" {
        GROUP "def" {
        }
    }
    GROUP "ghi" {
        GROUP "jkl" {
            GROUP "mno" {
                GROUP "BCD" {
                }
                GROUP "EFG" {
                }
                GROUP "HIJ" {
                }
                GROUP "KLM" {
                }
                GROUP "pqr" {
                }
                GROUP "stu" {
                }
                GROUP "vwx" {
                }
                GROUP "yzA" {
                }
            }
        }
    }
}
}
