/*
 * Copyright (C) 2011-2017 Paul Davis <paul@linuxaudiosystems.com>
 * Copyright (C) 2024 Ben Loftis <ben@harrisonconsoles.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "pbd/compose.h"
#include <algorithm>

#include "gtkmm2ext/actions.h"
#include "gtkmm2ext/gui_thread.h"
#include "gtkmm2ext/utils.h"

#include "ardour/location.h"
#include "ardour/profile.h"
#include "ardour/session.h"

#include "audio_clock.h"
#include "editor_automation_line.h"
#include "control_point.h"
#include "editor.h"
#include "region_view.h"

#include "audio_region_properties_box.h"

#include "pbd/i18n.h"

using namespace Gtk;
using namespace ARDOUR;
using namespace ArdourWidgets;
using std::max;
using std::min;

RegionPropertiesBox::RegionPropertiesBox ()
	: _region_ed(0)
{
	pack_start (_reged_box, true, false);
	_reged_box.show();
}

RegionPropertiesBox::~RegionPropertiesBox ()
{
}

void
RegionPropertiesBox::set_session (Session* s)
{
	SessionHandlePtr::set_session (s);

	if (s) {
		return;
	}
}

void
RegionPropertiesBox::set_regionview (RegionView *rv)
{
	std::shared_ptr<Region> r = rv->region();

	if (_region_ed) {
		_reged_box.remove(*_region_ed);
		delete _region_ed;
	}

	_region_ed = manage (new RegionEditor(&r->session(), rv));
	_reged_box.pack_start(*_region_ed, false, false);

	_reged_box.show();
	_region_ed->show();

	set_session (&r->session ());

	state_connection.disconnect ();

	_region = r;

	PBD::PropertyChange interesting_stuff;
	region_changed (interesting_stuff);

	_region->PropertyChanged.connect (state_connection, invalidator (*this), std::bind (&RegionPropertiesBox::region_changed, this, _1), gui_context ());
}

void
RegionPropertiesBox::region_changed (const PBD::PropertyChange& what_changed)
{
}