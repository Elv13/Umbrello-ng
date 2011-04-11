/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2006-2009                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

#ifndef UMLVIEWIMAGEEXPORTER_H
#define UMLVIEWIMAGEEXPORTER_H

#include <QtCore/QString>
#include <kurl.h>

class UMLView;
class KFileDialog;

/**
 * Exports the view as an image.
 * This class takes care of asking the user the needed parameters and
 * then exports the view.
 */
class UMLViewImageExporter
{
public:

    UMLViewImageExporter(UMLView* view);
    virtual ~UMLViewImageExporter();

    void exportView();

    KUrl    getImageURL() const { return m_imageURL; }
    QString getImageMimeType() const { return m_imageMimeType; }

private:

    UMLView* m_view;           ///< The view to export.
    KUrl     m_imageURL;       ///< The URL used to save the image.
    QString  m_imageMimeType;  ///< The mime type used to save the image.

    bool getParametersFromUser();

    bool prepareExportView();
    void prepareFileDialog(KFileDialog *fileDialog);

};

#endif
