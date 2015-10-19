back(t)
	Trans *t;
{
	switch (t->back) {
	default: Uerror("bad return move");
	case  0: return; /* nothing to undo */

		 /* PROC _init */
	case  1: /* STATE 1 */
		;
		delproc(0, now._nr_pr-1);
		return;
	case  2: /* STATE 2 */
		;
		delproc(0, now._nr_pr-1);
		return;
	case  3: /* STATE 3 */
		;
		delproc(0, now._nr_pr-1);
		return;
	case  4: /* STATE 4 */
		;
		delproc(0, now._nr_pr-1);
		return;
	case  5: /* STATE 5 */
		p_restor(II);
		return;

		 /* PROC Y2 */
	case  6: /* STATE 1 */
		now.y = trpt->oval;
		return;
	case  7: /* STATE 2 */
		p_restor(II);
		return;

		 /* PROC Y1 */
	case  8: /* STATE 1 */
		now.y = trpt->oval;
		return;
	case  9: /* STATE 2 */
		now.y = trpt->oval;
		return;
	case  10: /* STATE 3 */
		p_restor(II);
		return;

		 /* PROC X2 */
	case  11: /* STATE 1 */
		now.x = trpt->oval;
		return;
	case  12: /* STATE 2 */
		p_restor(II);
		return;

		 /* PROC X1 */
	case  13: /* STATE 1 */
		now.x = trpt->oval;
		return;
	case  14: /* STATE 2 */
		now.x = trpt->oval;
		return;
	case  15: /* STATE 3 */
		p_restor(II);
		return;
	}

}

