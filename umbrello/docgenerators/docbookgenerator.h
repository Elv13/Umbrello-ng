/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2006      Gael de Chalendar (aka Kleag) kleag@free.fr   *
 *   copyright (C) 2006-2007                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

#ifndef DOCBOOKGENERATOR_H
#define DOCBOOKGENERATOR_H

#include <kurl.h>
#include <QtCore/QObject>

class UMLDoc;
class DocbookGeneratorJob;

/**
 * class DocbookGenerator is a documentation generator for UML documents.
 * It uses libxslt to convert the XMI generated by UMLDoc::saveToXMI through
 * the XSLT file stored in resources.
 *
 * @todo Add configure checks for libxml2 and libxslt and use conditional
 * compilation of this library and its callers
 * @todo allow to specify the destination and ensure that it works with distant
 * ones
 */
class DocbookGenerator : public QObject
{
    Q_OBJECT
  public:

    DocbookGenerator();
    virtual ~DocbookGenerator();

    bool generateDocbookForProject();
    void generateDocbookForProjectInto(const KUrl& destDir);

  signals:

    void finished(bool status);

  private slots:

    void slotDocbookGenerationFinished(const QString&);

    void threadFinished();

  private:

    DocbookGeneratorJob* docbookGeneratorJob;

    bool m_pStatus;
    bool m_pThreadFinished;
    KUrl m_destDir;
    UMLDoc* umlDoc;
};

#endif // DOCBOOKGENERATOR_H