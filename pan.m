move(t,c)
	Trans *t;
	uchar c;
{
	switch (t->forw) {
	default: Uerror("bad forward move");

		 /* PROC _init */
	case  1: /* STATE 1 - type 99 - line 26 */
		addproc(4, now.x);
		break;
	case  2: /* STATE 2 - type 99 - line 27 */
		addproc(3, now.x);
		break;
	case  3: /* STATE 3 - type 99 - line 28 */
		addproc(2, now.y);
		break;
	case  4: /* STATE 4 - type 99 - line 30 */
		addproc(1, now.y);
		break;
	case  5: /* STATE 5 - type 64 - line 30 */
		delproc(c, II);
		break;

		 /* PROC Y2 */
	case  6: /* STATE 1 - type 265 - line 22 */
		if(c) (trpt+1)->oval = now.y;
		now.y = (now.y+3);
		break;
	case  7: /* STATE 2 - type 64 - line 23 */
		delproc(c, II);
		break;

		 /* PROC Y1 */
	case  8: /* STATE 1 - type 265 - line 16 */
		if(c) (trpt+1)->oval = now.y;
		now.y = (now.y+1);
		break;
	case  9: /* STATE 2 - type 265 - line 17 */
		if(c) (trpt+1)->oval = now.y;
		now.y = (now.y*2);
		break;
	case  10: /* STATE 3 - type 64 - line 18 */
		delproc(c, II);
		break;

		 /* PROC X2 */
	case  11: /* STATE 1 - type 265 - line 11 */
		if(c) (trpt+1)->oval = now.x;
		now.x = (now.x+3);
		break;
	case  12: /* STATE 2 - type 64 - line 12 */
		delproc(c, II);
		break;

		 /* PROC X1 */
	case  13: /* STATE 1 - type 265 - line 5 */
		if(c) (trpt+1)->oval = now.x;
		now.x = (now.x+1);
		break;
	case  14: /* STATE 2 - type 265 - line 6 */
		if(c) (trpt+1)->oval = now.x;
		now.x = (now.x+2);
		break;
	case  15: /* STATE 3 - type 64 - line 7 */
		delproc(c, II);
		break;
	}
}

