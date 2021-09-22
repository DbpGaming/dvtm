static void xstack(int stack)
{
	unsigned int i, n, nx, ny, nw, nh, m, mw, mh, tw, th;
	Client *c;

	for (n = 0, c = nextvisible(clients); c; c = nextvisible(c->next))
		if (!c->minimized)
			n++;

	m  = MAX(1, MIN(n, screen.nmaster));
	nx = wax;

	if (stack == 1){
		mw = n == m ? waw : screen.mfact * waw;
		mh = wah / m;
		th = n == m ? 0 : wah / (n - m);
	} else {
		mh = n == m ? wah : screen.mfact * wah;
		if (stack == 2)
			th = n == m ? 0 : (wah - mh) / (n - m);
		else
			mw = waw / m;
		tw = n == m ? 0 : waw / (n - m);
	}

	if (stack == 3)
		ny = way + wah - mh;
	else
		ny = way;

	for (i = 0, c = nextvisible(clients); c; c = nextvisible(c->next)) {
		if (c->minimized)
			continue;
		if (i < m) {	/* master */
			if (stack == 2)
				nh = (i < m - 1) ? mh / m : (way + mh) - ny;
			else if(stack == 1){
				nw = mw;
				nh = (i < m - 1) ? mh : (way + wah) - ny;
			}else{
				if (i > 0) {
					mvvline(ny, nx, ACS_VLINE, nh);
					mvaddch(ny, nx, ACS_TTEE);
					nx++;
				}
				nh = mh;
				nw = (i < m - 1) ? mw : (wax + waw) - nx;
			}
		} else {	/* tile window */
			if (stack == 2)
				nh = (i < n - 1) ? th : (way + wah) - ny;
			else {
				if (i == m) {
					nx = wax;
					if(stack == 3) {
						ny = way;
						nh = (way + wah) - ny - mh;
					} else if (stack == 1){
						ny = way;
						nx += mw;
						mvvline(ny, nx, ACS_VLINE, wah);
						mvaddch(ny, nx, ACS_TTEE);
						nx++;
						nw = waw - mw -1;
					} else {
						ny += mh;
						nh = (way + wah) - ny;
					}
				}
				if (i > m)
					if (stack == 1)
						mvaddch(ny, nx - 1, ACS_LTEE);
					else {
						mvvline(ny, nx, ACS_VLINE, nh);
						mvaddch(ny, nx, ACS_TTEE);
						nx++;
					}
				if (stack == 1)
					nh = (i < n - 1) ? th : (way + wah) - ny;
				else
					nw = (i < n - 1) ? tw : (wax + waw) - nx;
				}
			}

		if (stack == 2)
			resize(c, wax, ny, waw, nh);
		else
			resize(c, nx, ny, nw, nh);

		if (stack == 1 | stack == 2)
			ny += nh;
		else
			nx += nw;
		i++;
	}

	/* Fill in nmaster intersections */
	if (n > m)
		if (stack == 1){
			ny = way + mh;
			for (i = 1; i < m; i++) {
				mvaddch(ny, nx - 1, ((ny - 1) % th ? ACS_RTEE : ACS_PLUS));
				ny += mh;
			}
		} else if (!stack == 3) {
			nx = wax;
			for (i = 0; i < m; i++) {
				if (i > 0) {
					mvaddch(ny, nx, ACS_PLUS);
					nx++;
				}
				nw = (i < m - 1) ? mw : (wax + waw) - nx;
				nx += nw;
			}
		}
}

static void bstack(void)
{
	xstack(0);
}

static void tstack(void)
{
	xstack(3);
}

static void vstack(void)
{
	xstack(2);
}

static void tile(void)
{
	xstack(1);
}

static void fullscreen(void)
{
	for (Client *c = nextvisible(clients); c; c = nextvisible(c->next))
		resize(c, wax, way, waw, wah);
}
