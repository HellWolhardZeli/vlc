package org.videolan.jvlc;

/*****************************************************************************
 * JVLC.java: global class for vlc Java Bindings
 *****************************************************************************
 * Copyright (C) 1998-2004 the VideoLAN team
 *
 * Authors: Filippo Carone <filippo@carone.org>
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
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

/**
 * This class is part of JVLC<Type>Variable pool.
 */
public class JVLCIntVariable extends JVLCVariable {


    public JVLCIntVariable(String name, int value) throws JVLCNoSuchVariableNameException {
        super(name);
        this.value = new Integer(value);
    }

    public int getIntValue() {
	return ((Integer)value).intValue();
    }

}
