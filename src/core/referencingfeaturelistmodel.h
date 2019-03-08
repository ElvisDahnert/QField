/***************************************************************************
  referencingfeaturelistmodel.h - ReferencingFeatureListModel

 ---------------------
 begin                : 1.3.2019
 copyright            : (C) 2019 by David Signer Kuhn
 email                : david@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef REFERENCINGFEATURELISTMODEL_H
#define REFERENCINGFEATURELISTMODEL_H

#include <QAbstractItemModel>
#include "qgsvectorlayer.h"
#include "attributeformmodel.h"

class QgsVectorLayer;
class AttributeFormModel;

class ReferencingFeatureListModel : public QStandardItemModel
{
  Q_OBJECT

  /**
   * The relation
   */
  Q_PROPERTY( AttributeFormModel *attributeFormModel READ attributeFormModel WRITE setAttributeFormModel NOTIFY attributeFormModelChanged )
  Q_PROPERTY( QgsFeatureId featureId WRITE setFeatureId READ featureId NOTIFY featureIdChanged )
  Q_PROPERTY( QgsRelation relation WRITE setRelation READ relation NOTIFY relationChanged )

public:
  explicit ReferencingFeatureListModel(QObject *parent = nullptr);

  enum ReferencedFeatureListRoles
  {
    DisplayString = Qt::UserRole,
    FeatureId
  };

  QHash<int, QByteArray> roleNames() const override;
  QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex &index) const override;
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

  void setFeatureId( const QgsFeatureId &featureId );
  QgsFeatureId featureId () const;

  void setRelation( const QgsRelation &relation );
  QgsRelation relation() const;

  AttributeFormModel *attributeFormModel() const;
  void setAttributeFormModel( AttributeFormModel *attributeFormModel );

  void reload();
  Q_INVOKABLE void deleteFeature( QgsFeatureId fid );

signals:
  void attributeFormModelChanged();
  void featureIdChanged();
  void relationChanged();

private:
  struct Entry
  {
    Entry( const QString &displayString, const QgsFeatureId &featureId )
      : displayString( displayString )
       , featureId(featureId)
    {}

    Entry() = default;

    QString displayString;
    QgsFeatureId featureId;
  };

  QList<Entry> mEntries;

  AttributeFormModel *mAttributeFormModel;
  QgsFeatureId mFeatureId=-1;
  QgsRelation mRelation;

};

#endif // REFERENCINGFEATURELISTMODEL_H
