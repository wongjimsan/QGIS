/***************************************************************************
     qgsproperty.cpp
     ---------------
    Date                 : January 2017
    Copyright            : (C) 2017 by Nyall Dawson
    Email                : nyall dot dawson at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsproperty.h"

#include "qgslogger.h"
#include "qgsexpression.h"
#include "qgsfeature.h"
#include "qgssymbollayerutils.h"
#include "qgscolorramp.h"
#include <qmath.h>


QgsPropertyDefinition::QgsPropertyDefinition()
    : mTypes( DataTypeString )
{}

QgsPropertyDefinition::QgsPropertyDefinition( const QString& name, const QString& description, QgsPropertyDefinition::StandardPropertyTemplate type )
    : mName( name )
    , mDescription( description )
    , mStandardType( type )
{
  switch ( mStandardType )
  {
    case Boolean:
      mTypes = DataTypeBoolean;
      mHelpText = QObject::tr( "bool [<b>1</b>=True|<b>0</b>=False]" );
      break;

    case Integer:
      mTypes = DataTypeNumeric;
      mHelpText = QObject::tr( "int [&lt;= 0 =&gt;]" );
      break;

    case IntegerPositive:
      mTypes = DataTypeNumeric;
      mHelpText = QObject::tr( "int [&gt;= 0]" );
      break;

    case IntegerPositiveGreaterZero:
      mTypes = DataTypeNumeric;
      mHelpText = QObject::tr( "int [&gt;= 1]" );
      break;

    case Double:
      mTypes = DataTypeNumeric;
      mHelpText = QObject::tr( "double [&lt;= 0.0 =&gt;]" );
      break;

    case DoublePositive:
      mTypes = DataTypeNumeric;
      mHelpText = QObject::tr( "double [&gt;= 0.0]" );
      break;

    case Double0To1:
      mTypes = DataTypeNumeric;
      mHelpText = QObject::tr( "double [0.0-1.0]" );
      break;

    case Rotation:
      mTypes = DataTypeNumeric;
      mHelpText = QObject::tr( "double [0.0-360.0]" );
      break;

    case String:
      mTypes = DataTypeString;
      mHelpText = QObject::tr( "string of variable length" );
      break;

    case Transparency:
      mTypes = DataTypeNumeric;
      mHelpText = QObject::tr( "int [0-100]" );
      break;

    case RenderUnits:
      mTypes = DataTypeString;
      mHelpText = trString() + QLatin1String( "[<b>MM</b>|<b>MapUnit</b>|<b>Pixel</b>|<b>Point</b>]" );
      break;

    case ColorWithAlpha:
      mTypes = DataTypeString;
      mHelpText = QObject::tr( "string [<b>r,g,b,a</b>] as int 0-255 or #<b>RRGGBBAA</b> as hex or <b>color</b> as color's name" );
      break;

    case ColorNoAlpha:
      mTypes = DataTypeString;
      mHelpText = QObject::tr( "string [<b>r,g,b</b>] as int 0-255 or #<b>RRGGBB</b> as hex or <b>color</b> as color's name" );
      break;

    case PenJoinStyle:
      mTypes = DataTypeString;
      mHelpText = trString() + QLatin1String( "[<b>bevel</b>|<b>miter</b>|<b>round</b>]" );
      break;

    case BlendMode:
      mTypes = DataTypeString;
      mHelpText = trString() + QLatin1String( "[<b>Normal</b>|<b>Lighten</b>|<b>Screen</b>|<b>Dodge</b>|<br>"
                                              "<b>Addition</b>|<b>Darken</b>|<b>Multiply</b>|<b>Burn</b>|<b>Overlay</b>|<br>"
                                              "<b>SoftLight</b>|<b>HardLight</b>|<b>Difference</b>|<b>Subtract</b>]" );
      break;

    case Point:
      mTypes = DataTypeString;
      mHelpText = QObject::tr( "double coord [<b>X,Y</b>]" );
      break;

    case Size:
      mTypes = DataTypeNumeric;
      mHelpText = QObject::tr( "double [&gt;= 0.0]" );
      break;

    case Size2D:
      mTypes = DataTypeString;
      mHelpText = QObject::tr( "double size [<b>width,height</b>]" );
      break;

    case LineStyle:
      mTypes = DataTypeString;
      mHelpText = trString() + QLatin1String( "[<b>no</b>|<b>solid</b>|<b>dash</b>|<b>dot</b>|<b>dash dot</b>|<b>dash dot dot</b>]" );
      break;

    case StrokeWidth:
      mTypes = DataTypeNumeric;
      mHelpText = QObject::tr( "double [&gt;= 0.0]" );
      break;

    case FillStyle:
      mTypes = DataTypeString;
      mHelpText = trString() + QLatin1String( "[<b>solid</b>|<b>horizontal</b>|<b>vertical</b>|<b>cross</b>|<b>b_diagonal</b>|<b>f_diagonal"
                                              "</b>|<b>diagonal_x</b>|<b>dense1</b>|<b>dense2</b>|<b>dense3</b>|<b>dense4</b>|<b>dense5"
                                              "</b>|<b>dense6</b>|<b>dense7</b>|<b>no]" );
      break;

    case CapStyle:
      mTypes = DataTypeString;
      mHelpText = trString() + QLatin1String( "[<b>square</b>|<b>flat</b>|<b>round</b>]" );
      break;

    case HorizontalAnchor:
      mTypes = DataTypeString;
      mHelpText = trString() + QLatin1String( "[<b>left</b>|<b>center</b>|<b>right</b>]" );
      break;

    case VerticalAnchor:
      mTypes = DataTypeString;
      mHelpText = trString() + QLatin1String( "[<b>top</b>|<b>center</b>|<b>bottom</b>]" );
      break;

    case SvgPath:
      mTypes = DataTypeString;
      mHelpText = trString() + QLatin1String( "[<b>filepath</b>] as<br>"
                                              "<b>''</b>=empty|absolute|search-paths-relative|<br>"
                                              "project-relative|URL" );
      break;

    case Offset:
      mTypes = DataTypeString;
      mHelpText = QObject::tr( "double offset [<b>x,y</b>]" );
      break;

    case Custom:
      mTypes = DataTypeString;
  }
}

QgsPropertyDefinition::QgsPropertyDefinition( const QString& name, DataType dataTypes, const QString& description, const QString& helpText )
    : mName( name )
    , mDescription( description )
    , mTypes( dataTypes )
    , mHelpText( helpText )
{}

bool QgsPropertyDefinition::supportsAssistant() const
{
  return mTypes == DataTypeNumeric || mStandardType == Size || mStandardType == StrokeWidth || mStandardType == ColorNoAlpha || mStandardType == ColorWithAlpha
         || mStandardType == Rotation;
}

QString QgsPropertyDefinition::trString()
{
  // just something to reduce translation redundancy
  return QObject::tr( "string " );
}

//
// QgsProperty
//

QgsProperty::QgsProperty()
{
  d = new QgsPropertyPrivate();
}

QgsProperty QgsProperty::fromExpression( const QString& expression, bool isActive )
{
  QgsProperty p;
  p.setExpressionString( expression );
  p.setActive( isActive );
  return p;
}

QgsProperty QgsProperty::fromField( const QString& fieldName, bool isActive )
{
  QgsProperty p;
  p.setField( fieldName );
  p.setActive( isActive );
  return p;
}

QgsProperty QgsProperty::fromValue( const QVariant& value, bool isActive )
{
  QgsProperty p;
  p.setStaticValue( value );
  p.setActive( isActive );
  return p;
}

QgsProperty::QgsProperty( const QgsProperty& other )
    : d( other.d )
{}

QgsProperty &QgsProperty::operator=( const QgsProperty & other )
{
  d = other.d;
  return *this;
}

bool QgsProperty::operator==( const QgsProperty& other ) const
{
  return d->active == other.d->active
         && d->type == other.d->type
         && ( d->type != StaticProperty || d->staticValue == other.d->staticValue )
         && ( d->type != FieldBasedProperty || d->fieldName == other.d->fieldName )
         && ( d->type != ExpressionBasedProperty || d->expressionString == other.d->expressionString )
         && (( !d->transformer && !other.d->transformer ) || ( d->transformer && other.d->transformer && d->transformer->toExpression( QString() ) == other.d->transformer->toExpression( QString() ) ) );
}

bool QgsProperty::operator!=( const QgsProperty& other ) const
{
  return ( !(( *this ) == other ) );
}

QgsProperty::Type QgsProperty::propertyType() const
{
  return static_cast< Type >( d->type );
}

bool QgsProperty::isActive() const
{
  return d->type != InvalidProperty && d->active;
}

void QgsProperty::setActive( bool active )
{
  d.detach();
  d->active = active;
}

void QgsProperty::setStaticValue( const QVariant& value )
{
  d.detach();
  d->type = StaticProperty;
  d->staticValue = value;
}

QVariant QgsProperty::staticValue() const
{
  if ( d->type != StaticProperty )
    return QVariant();

  return d->staticValue;
}

void QgsProperty::setField( const QString& field )
{
  d.detach();
  d->type = FieldBasedProperty;
  d->fieldName = field;
  d->cachedFieldIdx = -1;
}

QString QgsProperty::field() const
{
  if ( d->type != FieldBasedProperty )
    return QString();

  return d->fieldName;
}

QgsProperty::operator bool() const
{
  return d->type != InvalidProperty;
}

void QgsProperty::setExpressionString( const QString& expression )
{
  d.detach();
  d->type = ExpressionBasedProperty;
  d->expressionString = expression;
  d->expression = QgsExpression( expression );
  d->expressionPrepared = false;
}

QString QgsProperty::expressionString() const
{
  if ( d->type != ExpressionBasedProperty )
    return QString();

  return d->expressionString;
}


QString QgsProperty::asExpression() const
{
  QString exp;
  switch ( d->type )
  {
    case StaticProperty:
      exp = QgsExpression::quotedValue( d->staticValue );
      break;

    case FieldBasedProperty:
      exp = QgsExpression::quotedColumnRef( d->fieldName );
      break;

    case ExpressionBasedProperty:
      exp = d->expressionString;
      break;

    case InvalidProperty:
      exp = QString();
      break;
  }
  return d->transformer ? d->transformer->toExpression( exp ) : exp;
}

bool QgsProperty::prepare( const QgsExpressionContext& context ) const
{
  if ( !d->active )
    return true;

  switch ( d->type )
  {
    case StaticProperty:
      return true;

    case FieldBasedProperty:
    {
      d.detach();
      // cache field index to avoid subsequent lookups
      QgsFields f = context.fields();
      d->cachedFieldIdx = f.lookupField( d->fieldName );
      return true;
    }

    case ExpressionBasedProperty:
    {
      d.detach();
      if ( !d->expression.prepare( &context ) )
      {
        d->expressionReferencedCols.clear();
        d->expressionPrepared = false;
        return false;
      }

      d->expressionPrepared = true;
      d->expressionReferencedCols = d->expression.referencedColumns();
      return true;
    }

    case InvalidProperty:
      return true;

  }

  return false;
}

QSet<QString> QgsProperty::referencedFields( const QgsExpressionContext& context ) const
{
  if ( !d->active )
    return QSet<QString>();

  switch ( d->type )
  {
    case StaticProperty:
    case InvalidProperty:
      return QSet<QString>();

    case FieldBasedProperty:
    {
      QSet< QString > fields;
      if ( !d->fieldName.isEmpty() )
        fields.insert( d->fieldName );
      return fields;
    }

    case ExpressionBasedProperty:
    {
      d.detach();
      if ( !d->expressionPrepared && !prepare( context ) )
        return QSet< QString >();

      return d->expressionReferencedCols;
    }

  }
  return QSet<QString>();
}

QVariant QgsProperty::propertyValue( const QgsExpressionContext& context, const QVariant& defaultValue, bool* ok ) const
{
  if ( ok )
    *ok = false;

  if ( !d->active )
    return defaultValue;

  switch ( d->type )
  {
    case StaticProperty:
    {
      if ( ok )
        *ok = true;
      return d->staticValue;
    }

    case FieldBasedProperty:
    {
      QgsFeature f = context.feature();
      if ( !f.isValid() )
        return defaultValue;

      //shortcut the field lookup
      if ( d->cachedFieldIdx >= 0 )
      {
        if ( ok )
          *ok = true;
        return f.attribute( d->cachedFieldIdx );
      }

      int fieldIdx = f.fieldNameIndex( d->fieldName );
      if ( fieldIdx < 0 )
        return defaultValue;

      if ( ok )
        *ok = true;
      return f.attribute( fieldIdx );
    }

    case ExpressionBasedProperty:
    {
      d.detach();
      if ( !d->expressionPrepared && !prepare( context ) )
        return defaultValue;

      QVariant result = d->expression.evaluate( &context );
      if ( result.isValid() )
      {
        if ( ok )
          *ok = true;
        return result;
      }
      else
      {
        return defaultValue;
      }
    }

    case InvalidProperty:
      return defaultValue;

  };

  return QVariant();
}


QVariant QgsProperty::value( const QgsExpressionContext& context, const QVariant& defaultValue, bool* ok ) const
{
  if ( ok )
    *ok = false;

  bool valOk = false;
  QVariant val = propertyValue( context, defaultValue, &valOk );
  if ( !d->transformer && !valOk ) // if transformer present, let it handle null values
    return defaultValue;

  if ( d->transformer )
  {
    if ( !valOk )
      val = QVariant();
    val = d->transformer->transform( context, val );
  }

  if ( ok )
    *ok = true;

  return val;
}

QString QgsProperty::valueAsString( const QgsExpressionContext& context, const QString& defaultString, bool* ok ) const
{
  bool valOk = false;
  QVariant val = value( context, defaultString, &valOk );

  if ( !valOk || !val.isValid() )
  {
    if ( ok )
      *ok = false;
    return defaultString;
  }
  else
  {
    if ( ok )
      *ok = true;
    return val.toString();
  }
}

QColor QgsProperty::valueAsColor( const QgsExpressionContext &context, const QColor &defaultColor, bool* ok ) const
{
  if ( ok )
    *ok = false;

  bool valOk = false;
  QVariant val = value( context, defaultColor, &valOk );

  if ( !valOk || !val.isValid() )
    return defaultColor;

  QColor color;
  if ( val.type() == QVariant::Color )
  {
    color = val.value<QColor>();
  }
  else
  {
    color = QgsSymbolLayerUtils::decodeColor( val.toString() );
  }

  if ( !color.isValid() )
    return defaultColor;
  else
  {
    if ( ok )
      *ok = true;
    return color;
  }
}

double QgsProperty::valueAsDouble( const QgsExpressionContext &context, double defaultValue, bool* ok ) const
{
  if ( ok )
    *ok = false;

  bool valOk = false;
  QVariant val = value( context, defaultValue, &valOk );

  if ( !valOk || !val.isValid() )
    return defaultValue;

  bool convertOk = false;
  double dbl = val.toDouble( &convertOk );
  if ( !convertOk )
    return defaultValue;
  else
  {
    if ( ok )
      *ok = true;
    return dbl;
  }
}

int QgsProperty::valueAsInt( const QgsExpressionContext &context, int defaultValue, bool* ok ) const
{
  if ( ok )
    *ok = false;

  bool valOk = false;
  QVariant val = value( context, defaultValue, &valOk );

  if ( !valOk || !val.isValid() )
    return defaultValue;

  bool convertOk = false;
  int integer = val.toInt( &convertOk );
  if ( !convertOk )
  {
    //one more option to try
    double dbl = val.toDouble( &convertOk );
    if ( convertOk )
    {
      if ( ok )
        *ok = true;
      return qRound( dbl );
    }
    else
    {
      return defaultValue;
    }
  }
  else
  {
    if ( ok )
      *ok = true;
    return integer;
  }
}

bool QgsProperty::valueAsBool( const QgsExpressionContext& context, bool defaultValue, bool* ok ) const
{
  if ( ok )
    *ok = false;

  bool valOk = false;
  QVariant val = value( context, defaultValue, &valOk );

  if ( !valOk || !val.isValid() )
    return defaultValue;

  if ( ok )
    *ok = true;
  return val.toBool();
}

bool QgsProperty::writeXml( QDomElement &propertyElem, QDomDocument &doc ) const
{
  propertyElem.setAttribute( "active", d->active ? "1" : "0" );
  propertyElem.setAttribute( "type", d->type );

  switch ( d->type )
  {
    case StaticProperty:
      propertyElem.setAttribute( "valType", d->staticValue.typeName() );
      propertyElem.setAttribute( "val", d->staticValue.toString() );
      break;

    case FieldBasedProperty:
      propertyElem.setAttribute( "field", d->fieldName );
      break;

    case ExpressionBasedProperty:
      propertyElem.setAttribute( "expression", d->expressionString );
      break;

    case InvalidProperty:
      break;
  }

  if ( d->transformer )
  {
    QDomElement transformerElem = doc.createElement( "transformer" );
    transformerElem.setAttribute( "t", static_cast< int >( d->transformer->transformerType() ) );
    if ( d->transformer->writeXml( transformerElem, doc ) )
      propertyElem.appendChild( transformerElem );
  }

  return true;
}

bool QgsProperty::readXml( const QDomElement &propertyElem, const QDomDocument &doc )
{
  d.detach();
  d->active = static_cast< bool >( propertyElem.attribute( "active", "1" ).toInt() );
  d->type = static_cast< Type >( propertyElem.attribute( "type", "0" ).toInt() );

  switch ( d->type )
  {
    case StaticProperty:
      d->staticValue = QVariant( propertyElem.attribute( "val", "" ) );
      d->staticValue.convert( QVariant::nameToType( propertyElem.attribute( "valType", "QString" ).toLocal8Bit().constData() ) );
      break;

    case FieldBasedProperty:
      d->fieldName = propertyElem.attribute( "field" );
      if ( d->fieldName.isEmpty() )
        d->active = false;
      break;

    case ExpressionBasedProperty:
      d->expressionString = propertyElem.attribute( "expression" );
      if ( d->expressionString.isEmpty() )
        d->active = false;

      d->expression = QgsExpression( d->expressionString );
      d->expressionPrepared = false;
      d->expressionReferencedCols.clear();
      break;

    case InvalidProperty:
      break;

  }

  //restore transformer if present
  if ( d->transformer )
    delete d->transformer;
  d->transformer = nullptr;
  QDomNodeList transformerNodeList = propertyElem.elementsByTagName( "transformer" );
  if ( !transformerNodeList.isEmpty() )
  {
    QDomElement transformerElem = transformerNodeList.at( 0 ).toElement();
    QgsPropertyTransformer::Type type = static_cast< QgsPropertyTransformer::Type >( transformerElem.attribute( "t", "0" ).toInt() );
    std::unique_ptr< QgsPropertyTransformer > transformer( QgsPropertyTransformer::create( type ) );
    if ( transformer )
    {
      if ( transformer->readXml( transformerElem, doc ) )
        d->transformer = transformer.release();
    }
  }

  return true;
}


void QgsProperty::setTransformer( QgsPropertyTransformer* transformer )
{
  d.detach();
  d->transformer = transformer;
}

const QgsPropertyTransformer* QgsProperty::transformer() const
{
  return d->transformer;
}

bool QgsProperty::convertToTransformer()
{
  if ( d->type != ExpressionBasedProperty )
    return false;

  if ( d->transformer )
    return false; // already a transformer

  QString baseExpression;
  QString fieldName;
  std::unique_ptr< QgsPropertyTransformer > transformer( QgsPropertyTransformer::fromExpression( d->expressionString, baseExpression, fieldName ) );
  if ( !transformer )
    return false;

  d.detach();
  d->transformer = transformer.release();
  if ( !fieldName.isEmpty() )
    setField( fieldName );
  else
    setExpressionString( baseExpression );
  return true;
}



