/*
 * Copyright (C) 2025 Paul Davis <paul@linuxaudiosystems.com>
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

#include "pbd/types_convert.h"
#include "pbd/error.h"
#include "pbd/xml++.h"

#include "region_ui_settings.h"

#include "pbd/i18n.h"

using namespace PBD;
using namespace Editing;

RegionUISettings::RegionUISettings ()
	: grid_type (GridTypeBeat)
	, draw_length (Temporal::Beats (1, 0))
	, samples_per_pixel (2048)
	, follow_playhead (true)
	, play_selection (true)
{
}

XMLNode&
RegionUISettings::get_state () const
{
	XMLNode* node = new XMLNode (X_("RegionUISetting"));
	return *node;
}

int
RegionUISettings::set_state (XMLNode const & state, int)
{
	return 0;
}

XMLNode&
RegionUISettingsManager::get_state () const
{
	XMLNode* node = new XMLNode (X_("RegionUISettings"));
	return *node;
}

int
RegionUISettingsManager::set_state (XMLNode const & state, int)
{
	return 0;
}

void
RegionUISettingsManager::save (std::string const & path)
{
	XMLTree state_tree;

	state_tree.set_root (&get_state());
	state_tree.set_filename (path);

	if (state_tree.write()) {
		error << string_compose (_("could not save region GUI settings to %1"), path) << endmsg;
	}
}

int
RegionUISettingsManager::load (std::string const & xmlpath)
{
	XMLTree state_tree;

	if (!state_tree.read (xmlpath)) {
		return -1;
	}

	XMLNode const & root (*state_tree.root());

	if (root.name() != X_("RegionUISettingsManager")) {
		return -1;
	}

	for (auto const & node : root.children()) {
		RegionUISettings rsu;
		PBD::ID id;
		node->get_property ("id", id);

		if (rsu.set_state (*node, 0) == 0) {
			insert (std::make_pair (id, rsu));
		}
	}

	return 0;
}
