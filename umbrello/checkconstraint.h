/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2002-2006                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

#ifndef CHECKCONSTRAINT_H
#define CHECKCONSTRAINT_H

#include "entityconstraint.h"
#include "umlnamespace.h"

/**
 * This class is used to set up information for a unique entity constraint.
 *
 * @short Sets up Check Constraint information for UMLEntities.
 * @author Sharan Rao
 * @see UMLObject UMLClassifierListItem UMLEntityConstraint
 * Bugs and comments to uml-devel@lists.sf.net or http://bugs.kde.org
 */
class UMLCheckConstraint : public UMLEntityConstraint {
     Q_OBJECT
public:

    /**
     * Sets up a constraint.
     *
     * @param parent    The parent of this UMLCheckConstraint.
     * @param name      The name of this UMLCheckConstraint.
     * @param id        The unique id given to this UMLCheckConstraint.
     */
    UMLCheckConstraint(UMLObject *parent, const QString& name,
                       Uml::IDType id = Uml::id_None );

    /**
     * Sets up a constraint.
     *
     * @param parent    The parent of this UMLCheckConstraint.
     */
    UMLCheckConstraint(UMLObject *parent);

    /**
     * Overloaded '==' operator
     */
    bool operator==(const UMLCheckConstraint &rhs);

    /**
     * destructor.
     */
    virtual ~UMLCheckConstraint();

    virtual void copyInto(UMLObject *lhs) const;

    virtual UMLObject* clone() const;

    QString toString(Uml::Signature_Type sig = Uml::st_NoSig);

    QString getFullyQualifiedName(const QString& separator = QString(),
                                  bool includeRoot = false) const;

    void saveToXMI( QDomDocument & qDoc, QDomElement & qElement );

    virtual bool showPropertiesDialog(QWidget* parent);

    QString getCheckCondition() const {
        return m_CheckCondition;
    }

    void setCheckCondition(const QString& condition) {
        m_CheckCondition = condition.trimmed();
    }

protected:

    bool load( QDomElement & element );

private:

    void init();

    /**
     * The Check Condition
     */
    QString m_CheckCondition;

};

#endif
