/***************************************************************************
                          qgsexception.h
                             -------------------
  begin                : August 31, 2004
  copyright            : (C) 2004 by Mark Coletti
  email                : mcoletti at gmail.com
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QGSEXCEPTION_H
#define QGSEXCEPTION_H

#include <QString>

#include "qgis_core.h"

/** \ingroup core
  * Defines a qgis exception class.
 */
class CORE_EXPORT QgsException
{
  public:
    QgsException( QString const & what )
        : what_( what )
    {}

    virtual ~QgsException() throw()
    {}

    //! @note not available in Python bindings
    QString what() const throw()
    {
      return what_;
    }

  private:

    /// description of exception
    QString what_;

}; // class QgsException


#endif
