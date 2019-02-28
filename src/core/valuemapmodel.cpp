#include "valuemapmodel.h"
#include "editorwidgetconfig.h"

ValueMapModel::ValueMapModel( QObject *parent )
  : QAbstractListModel( parent )
{
}

QVariant ValueMapModel::map() const
{
  return mConfiguredMap;
}

void ValueMapModel::setMap( const QVariant &config )
{
  QVariant map = config.value<EditorWidgetConfig>().configuration().value( QStringLiteral( "map" ) );
  mMap.clear();
  // QGIS 3
  const QVariantList list = map.toList();
  if ( !list.empty() )
  {
    beginInsertRows( QModelIndex(), 0, list.size() );

    for ( const QVariant &item : list )
    {
      const QVariantMap mapItem = item.toMap();

      const QString key = mapItem.firstKey();
      const QVariant value = mapItem.value( key );

      mMap.append( qMakePair( value, key ) );
    }
    endInsertRows();
  }
  else // QGIS 2 compat
  {
    const QVariantMap valueMap = map.toMap();
    if ( !valueMap.empty() )
    {
      beginInsertRows( QModelIndex(), 0, valueMap.size() );

      QMapIterator<QString, QVariant> i( valueMap );
      while ( i.hasNext() )
      {
        i.next();
        const QString key = i.key();
        const QVariant value = i.value();

        mMap.append( qMakePair( value, key ) );
      }
      endInsertRows();
    }
  }

  mConfiguredMap = map;
  emit mapChanged();
}

int ValueMapModel::rowCount( const QModelIndex &parent ) const
{
  Q_UNUSED( parent )
  return mMap.size();
}

QVariant ValueMapModel::data( const QModelIndex &index, int role ) const
{
  if ( role == KeyRole )
    return mMap.at( index.row() ).first;
  else
    return mMap.at( index.row() ).second;
}

QHash<int, QByteArray> ValueMapModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();

  roles[KeyRole] = "key";
  roles[ValueRole] = "value";

  return roles;
}

int ValueMapModel::keyToIndex( const QVariant &value ) const
{
  int i = 0;
  for ( const auto &item : mMap )
  {
    if ( item.first.toString() == value.toString() )
    {
      return i;
    }
    ++i;
  }
  return -1;
}

QVariant ValueMapModel::keyForValue( const QString &value ) const
{
  QVariant result;
  for ( const auto &item : mMap )
  {
    if ( item.second == value )
      result = item.first;
  }
  return result;
}

