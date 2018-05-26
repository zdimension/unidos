
uint8_t cur_drive = 2;
struct dospath cur_path[26];

    memset(cur_path, 0, 26 * sizeof(struct dospath));
    for (int i = 0; i < 26; i++)
    {
        cur_path[i].drive = i;
    }
        case 0x0b: // check standard input status
        {
            uint8_t r_al = feof(stdin) ? 0x00 : 0xff;

            uc_reg_write(uc, UC_X86_REG_AL, &r_al);

            break;
        }

        case 0x0e: // select disk
        {
            uint8_t r_dl;

            uc_reg_read(uc, UC_X86_REG_DL, &r_dl);

            cur_drive = r_dl;

            uint8_t r_al = 1;

            uc_reg_write(uc, UC_X86_REG_AL, &r_al);

            break;
        }

        case 0x19: // get current default drive
        {
            uint8_t r_al = cur_drive;

            uc_reg_write(uc, UC_X86_REG_AL, &r_al);

            break;
        }


        case 0x30: // return DOS version 7.0
            {
                uint8_t r_al = 6;   // major
                uint8_t r_ah = 22;  // minor

                uc_reg_write(uc, UC_X86_REG_AL, &r_al);
                uc_reg_write(uc, UC_X86_REG_AH, &r_ah);

                break;
            }

        case 0x3b: // change current directory
        {
            uint16_t r_dx, r_ds;
            char *chdir;

            uc_reg_read(uc, UC_X86_REG_DX, &r_dx);
            uc_reg_read(uc, UC_X86_REG_DS, &r_ds);

            // read until '$'
            chdir = read_str_till_char(uc, MK_FP(r_ds, r_dx), '$');

            struct dospath res;
            string_to_path(chdir, &res);

            if (res.drive != -1)
            {
                cur_drive = res.drive;
            }
            else
            {
                res.drive = cur_drive;
            }

            cur_path[cur_drive] = res;

            set_flag_C(uc, 0);
        }


        case 0x2a: // get system date
        {
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);

            uint16_t r_cx = tm.tm_year - 80;
            uint8_t r_dh = tm.tm_mon + 1;
            uint8_t r_dl = tm.tm_mday;
            uint8_t r_al = tm.tm_wday;

            uc_reg_write(uc, UC_X86_REG_CX, &r_cx);
            uc_reg_write(uc, UC_X86_REG_DH, &r_dh);
            uc_reg_write(uc, UC_X86_REG_DL, &r_dl);
            uc_reg_write(uc, UC_X86_REG_AL, &r_al);

            break;
        }

        case 0x2c: // get system time
        {
            struct timeval tval;
            gettimeofday(&tval, NULL);
            struct tm tm = *localtime(&tval.tv_sec);

            uint8_t r_ch = tm.tm_hour;
            uint8_t r_cl = tm.tm_min;
            uint8_t r_dh = tval.tv_sec;
            uint8_t r_dl = tval.tv_usec / 10000;

            uc_reg_write(uc, UC_X86_REG_CH, &r_ch);
            uc_reg_write(uc, UC_X86_REG_CL, &r_cl);
            uc_reg_write(uc, UC_X86_REG_DH, &r_dh);
            uc_reg_write(uc, UC_X86_REG_DL, &r_dl);

            break;
        }

        case 0x47: // get current directory
        {
            uint8_t r_dl;
            uc_reg_read(uc, UC_X86_REG_DL, &r_dl);

            if (r_dl == 0)
                r_dl = cur_drive;
            else
                r_dl--;

            char* buf = malloc(66);
            path_to_string(cur_path[r_dl], buf);

            uint16_t r_ds, r_si;

            uc_reg_read(uc, UC_X86_REG_DS, &r_ds);
            uc_reg_read(uc, UC_X86_REG_SI, &r_si);

            uc_mem_write(uc, MK_FP(r_ds, r_si), buf + 2, 64);

            break;
        }
