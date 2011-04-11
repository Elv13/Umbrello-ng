/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2003-2010                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

#ifndef DOCKMANAGER_H
#define DOCKMANAGER_H

#include "classpropdock.h"
/**
 * @author Emmanuel Lepage Vallee <elv1313@gmail.com>
 * @version 1.0
 * Bugs and comments to uml-devel@lists.sf.net or http://bugs.kde.org
 */
class DockManager
{
public:
    static ClassPropDock* getClassPropDock() {
      if (!m_pClassPropDock) {
        m_pClassPropDock = new ClassPropDock();
      }
      return m_pClassPropDock;
    }
    virtual ~DockManager() {}

private:
    static ClassPropDock* m_pClassPropDock;

};


#endif
