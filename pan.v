setvar(type,h)
	short type;
	short h;
	enum{ nil,_var_timeout, x, y, First_queue};
{
switch (type) {

		 /* PROC _init */
	case 0:
	(ct[h][1-first_tr[type]][0]).var=x;
	(ct[h][1-first_tr[type]][0]).type=2;
	(ct[h][1-first_tr[type]][1]).var=0;
	(ct[h][2-first_tr[type]][0]).var=x;
	(ct[h][2-first_tr[type]][0]).type=2;
	(ct[h][2-first_tr[type]][1]).var=0;
	(ct[h][3-first_tr[type]][0]).var=y;
	(ct[h][3-first_tr[type]][0]).type=2;
	(ct[h][3-first_tr[type]][1]).var=0;
	(ct[h][4-first_tr[type]][0]).var=y;
	(ct[h][4-first_tr[type]][0]).type=2;
	(ct[h][4-first_tr[type]][1]).var=0;
	(ct[h][5-first_tr[type]][0]).var=0;
	break;

		 /* PROC Y2 */
	case 1:
	(ct[h][6-first_tr[type]][0]).var=y;
	(ct[h][6-first_tr[type]][0]).type=1;
	(ct[h][6-first_tr[type]][1]).var=y;
	(ct[h][6-first_tr[type]][1]).type=2;
	(ct[h][6-first_tr[type]][2]).var=0;
	(ct[h][7-first_tr[type]][0]).var=0;
	break;

		 /* PROC Y1 */
	case 2:
	(ct[h][8-first_tr[type]][0]).var=y;
	(ct[h][8-first_tr[type]][0]).type=1;
	(ct[h][8-first_tr[type]][1]).var=y;
	(ct[h][8-first_tr[type]][1]).type=2;
	(ct[h][8-first_tr[type]][2]).var=0;
	(ct[h][9-first_tr[type]][0]).var=y;
	(ct[h][9-first_tr[type]][0]).type=1;
	(ct[h][9-first_tr[type]][1]).var=y;
	(ct[h][9-first_tr[type]][1]).type=2;
	(ct[h][9-first_tr[type]][2]).var=0;
	(ct[h][10-first_tr[type]][0]).var=0;
	break;

		 /* PROC X2 */
	case 3:
	(ct[h][11-first_tr[type]][0]).var=x;
	(ct[h][11-first_tr[type]][0]).type=1;
	(ct[h][11-first_tr[type]][1]).var=x;
	(ct[h][11-first_tr[type]][1]).type=2;
	(ct[h][11-first_tr[type]][2]).var=0;
	(ct[h][12-first_tr[type]][0]).var=0;
	break;

		 /* PROC X1 */
	case 4:
	(ct[h][13-first_tr[type]][0]).var=x;
	(ct[h][13-first_tr[type]][0]).type=1;
	(ct[h][13-first_tr[type]][1]).var=x;
	(ct[h][13-first_tr[type]][1]).type=2;
	(ct[h][13-first_tr[type]][2]).var=0;
	(ct[h][14-first_tr[type]][0]).var=x;
	(ct[h][14-first_tr[type]][0]).type=1;
	(ct[h][14-first_tr[type]][1]).var=x;
	(ct[h][14-first_tr[type]][1]).type=2;
	(ct[h][14-first_tr[type]][2]).var=0;
	(ct[h][15-first_tr[type]][0]).var=0;
	break;

	}
}

