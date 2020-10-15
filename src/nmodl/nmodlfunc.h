int in_solvefor();
int cvode_cnexp_success();

void cvode_parse();
void Unit_push();
void unit_pop();
void unit_div();
void install_units();
void modl_units();
void vectorize_if_else_stmt();
void consistency();
void c_out(const char* prefix);
void printlist();
void c_out_vectorize();
void vectorize_substitute();
void vectorize_do_substitute();
void solv_diffeq();
void add_deriv_imp_list();
void deriv_used();
void massagederiv();
void matchinitial();
void matchbound();
void checkmatch();
void matchmassage();
void copyitems();
void disc_var_seen();
void massagediscblk();
void init_disc_vars();
void init();
void inblock();
void unGets();
void diag();
void enquextern();
void include_file();
void reactname();
void leftreact();
void massagereaction();
void flux();
void massagekinetic();
void fixrlst();
void kinetic_intmethod();
void genfluxterm();
void kinetic_implicit();
void massageconserve();
void check_block();
void massagecompart();
void massageldifus();
void kin_vect1();
void kin_vect2();
void kin_vect3();
void ostmt_start();
void see_astmt();
void see_ostmt();
void prn();
void cvode_kinetic();
void single_channel();
void freelist();
void delete();
void deltokens();
void move();
void movelist();
void replacstr();
void c_out();
void printitem();
void debugprintitem();
void printlist();
void c_out_vectorize();
void vectorize_substitute();
void vectorize_do_substitute();
void nrninit();
void parout();
void warn_ignore();
void ldifusreg();
void decode_ustr();
void units_reg();
void nrn_list();
void bablk();
void nrn_use();
void nrn_var_assigned();
void slist_data();
void out_nt_ml_frag();
void cvode_emit_interface();
void cvode_proced_emit();
void cvode_interface();
void cvode_valid();
void cvode_rw_cur();
void net_receive();
void net_init();
void fornetcon();
void chk_thread_safe();
void threadsafe_seen();
void explicit_decl();
void parm_array_install();
void parminstall();
void steppedinstall();
void indepinstall();
void depinstall();
void statdefault();
void vectorize_scan_for_func();
void defarg();
void lag_stmt();
void queue_stmt();
void add_reset_args();
void add_nrnthread_arg();
void check_tables();
void table_massage();
void hocfunchack();
void hocfunc();
void vectorize_use_func();
void function_table();
void watchstmt();
void threadsafe();
void nrnmutex();
void solv_partial();
void partial_eqn();
void massagepartial();
void partial_bndry();
void sensparm();
void add_sens_statelist();
void sensmassage();
void sens_nonlin_out();
void solv_nonlin();
void solv_lineq();
void eqnqueue();
void massagenonlin();
void init_linblk();
void init_lineq();
void lin_state_term();
void linterm();
void massage_linblk();
void solvequeue();
void solvhandler();
void save_dt();
void symbol_init();
void pushlocal();
void poplocal();

void conductance_hint(int blocktype, Item* q1, Item* q2);
void possible_local_current(int blocktype, List* symlist);
Symbol* breakpoint_current(Symbol* s);
void netrec_asgn(Item* varname, Item* equal, Item* expr, Item* lastok);
void netrec_discon();
char* items_as_string(Item* begin, Item* last); /* does not include last */
int slist_search(int listnum, Symbol* s);
void nrnunit_dynamic_str(char* buf, const char* name, char* unit1, char* unit2);
