precond(t)
	Trans *t;
{
	switch (t->forw) {
	default: Uerror("bad forward move");

		 /* PROC _init */
	case  1: 
		IfNotBlocked
		if (!(1))
			return 0;
		break;
	case  2: 
		IfNotBlocked
		if (!(1))
			return 0;
		break;
	case  3: 
		IfNotBlocked
		if (!(1))
			return 0;
		break;
	case  4: 
		IfNotBlocked
		if (!(1))
			return 0;
		break;
	case  5: 
		IfNotBlocked
		return 0;
		break;

		 /* PROC Y2 */
	case  6: 
		IfNotBlocked
		;
		break;
	case  7: 
		IfNotBlocked
		return 0;
		break;

		 /* PROC Y1 */
	case  8: 
		IfNotBlocked
		;
		break;
	case  9: 
		IfNotBlocked
		;
		break;
	case  10: 
		IfNotBlocked
		return 0;
		break;

		 /* PROC X2 */
	case  11: 
		IfNotBlocked
		;
		break;
	case  12: 
		IfNotBlocked
		return 0;
		break;

		 /* PROC X1 */
	case  13: 
		IfNotBlocked
		;
		break;
	case  14: 
		IfNotBlocked
		;
		break;
	case  15: 
		IfNotBlocked
		return 0;
		break;
	}
return 1;
}

