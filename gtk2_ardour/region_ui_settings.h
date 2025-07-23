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

#pragma once

#include <map>
#include <string>

#include "temporal/beats.h"

#include "editing.h"

class XMLNode;

struct RegionUISettings
{
	RegionUISettings ();

	Editing::GridType grid_type;
	Temporal::Beats draw_length;
	double samples_per_pixel;
	bool   follow_playhead;
	bool   play_selection;

	XMLNode& get_state () const;
	int set_state (XMLNode const & state, int version);
};

class RegionUISettingsManager : public std::map<PBD::ID, RegionUISettings> {
  public:
	RegionUISettingsManager () {}

	void save (std::string const & path);
	int load (std::string const & path);

	XMLNode& get_state () const;
	int set_state (XMLNode const & state, int version);
};
