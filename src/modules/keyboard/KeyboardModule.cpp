/*
 *  This file is part of XeroLinux Settings Manager.
 *
 *  Roland Singer <roland@manjaro.org>
 *  Ramon Buldó <ramon@manjaro.org>
 *
 *  XeroLinux Settings Manager is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  XeroLinux Settings Manager is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XeroLinux Settings Manager.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "KeyboardCommon.h"
#include "KeyboardModule.h"
#include "MsmCommon.h"
#include "ui_PageKeyboard.h"

#include <KAboutData>
#include <KPluginFactory>

#include <QTranslator>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>

#include <QDebug>

K_PLUGIN_FACTORY( MsmKeyboardFactory,
                  registerPlugin<KeyboardModule>( KeyboardCommon::getName() ); )

KeyboardModule::KeyboardModule( QWidget* parent, const QVariantList& args ) :
    KCModule( parent, args ),
    ui( new Ui::PageKeyboard ),
    m_keyboardModel( new KeyboardModel ),
    m_keyboardProxyModel( new QSortFilterProxyModel ),
    m_keyboardPreview( new KeyBoardPreview )
{
    Q_INIT_RESOURCE( translations );
    QTranslator* appTranslator = new QTranslator;
    appTranslator->load( ":/translations/msm_" + QLocale::system().name() );
    qApp->installTranslator( appTranslator );

    KAboutData* aboutData = new KAboutData( KeyboardCommon::getName(),
                                            KeyboardCommon::getTitle(),
                                            PROJECT_VERSION,
                                            KeyboardCommon::getDescription(),
                                            KAboutLicense::LicenseKey::GPL_V3,
                                            "(c) 2014 - 2017 XeroLinux Settings Manager developers" );
    aboutData->addAuthor( "Ramon Buldó",
                          QStringLiteral( "ramon@manjaro.org" ) );
    aboutData->addAuthor( "Roland Singer",
                          QStringLiteral( "roland@manjaro.org" ) );
    aboutData->setCustomAuthorText( QString(), MsmCommon::getBugReportLink() );
    setAboutData( aboutData );
    setButtons( KCModule::Default | KCModule::Apply );
    // hide restore button as it isn't connected or used in kcmodule.
    ui->setupUi( this );
    ui->buttonRestore->setVisible( false );

    // Keyboard preview widget
    ui->KBPreviewLayout->addWidget( m_keyboardPreview );
    m_keyboardPreview->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    // Connect signals and slots
    connect( ui->modelComboBox,
             static_cast<void ( QComboBox::* )( int )>( &QComboBox::currentIndexChanged ),
             [=] ( int index )
    {
        Q_UNUSED(index);
        QString model = ui->modelComboBox->itemData(
                            ui->modelComboBox->currentIndex(),
                            KeyboardModel::KeyRole )
                        .toString();
        m_keyboardModel->setNewModel( model );
    } );
    connect( ui->buttonRestore, &QPushButton::clicked,
             this, &KeyboardModule::defaults );
    connect( ui->layoutsListView, &QListView::clicked,
             this, &KeyboardModule::setNewLayout );
    connect( ui->variantsListView, &QListView::clicked,
             this, &KeyboardModule::setNewVariant );
    connect( m_keyboardModel, &KeyboardModel::changed,
             [=] ()
    {
        m_keyboardPreview->setLayout( m_keyboardModel->newLayout() );
        m_keyboardPreview->setVariant( m_keyboardModel->newVariant() );
        this->changed();
    } );
    connect( ui->sliderDelay, &QSlider::valueChanged,
             [=] ( int value )
    {
        m_keyboardModel->setNewDelay( value );
        ui->label_2->setText( QString::number( value ) );
    } );
    connect( ui->sliderRate, &QSlider::valueChanged,
             [=] ( int value )
    {
        m_keyboardModel->setNewRate( value );
        ui->label_3->setText( QString::number( value ) );
    } );
    // Setup Layout and Variant QListViews
    m_keyboardProxyModel->setSourceModel( m_keyboardModel );
    m_keyboardProxyModel->setSortLocaleAware( true );
    m_keyboardProxyModel->setSortRole( KeyboardModel::DescriptionRole );
    m_keyboardProxyModel->sort( 0, Qt::AscendingOrder );
    ui->layoutsListView->setModel( m_keyboardProxyModel );

    // Find root layout index and set it in the layoutsRootView
    QModelIndexList layoutsRootList = m_keyboardProxyModel->match( m_keyboardProxyModel->index( 0,0 ),
                                      KeyboardModel::KeyRole,
                                      "layouts",
                                      Qt::MatchFixedString );
    if ( layoutsRootList.size() == 1 )
    {
        QModelIndex layoutsRoot = layoutsRootList.first();
        ui->layoutsListView->setRootIndex( layoutsRoot );
    }
    else
        qDebug() << "Can't find keyboard layout list";

    m_layoutsSelectionProxy = new KSelectionProxyModel( ui->layoutsListView->selectionModel(), this );
    m_layoutsSelectionProxy->setSourceModel( m_keyboardModel );
    m_layoutsSelectionProxy->setFilterBehavior( KSelectionProxyModel::ChildrenOfExactSelection );

    m_variantsSortProxy = new QSortFilterProxyModel();
    m_variantsSortProxy->setSourceModel( m_layoutsSelectionProxy );
    m_variantsSortProxy->setSortLocaleAware( true );
    m_variantsSortProxy->sort( 0, Qt::AscendingOrder );
    ui->variantsListView->setModel( m_variantsSortProxy );

    // Set root index to the model combo box
    ui->modelComboBox->setModel( m_keyboardProxyModel );
    QModelIndexList modelsRootList = m_keyboardProxyModel->match( m_keyboardProxyModel->index( 0,0 ),
                                     KeyboardModel::KeyRole,
                                     "models", Qt::MatchFixedString );
    if ( modelsRootList.size() == 1 )
    {
        QModelIndex modelsRoot = modelsRootList.first();
        ui->modelComboBox->setRootModelIndex( modelsRoot );
    }
    else
        qDebug() << "Can't find keyboard model list";
}


KeyboardModule::~KeyboardModule()
{
    delete ui;
    delete m_keyboardModel;
    delete m_keyboardProxyModel;
    delete m_keyboardPreview;
    delete m_layoutsSelectionProxy;
    delete m_variantsSortProxy;
}


void
KeyboardModule::save()
{
    m_keyboardModel->saveKeyboardLayout();
    m_keyboardModel->saveRateAndDelay();
}


void
KeyboardModule::defaults()
{
    load();
}


void
KeyboardModule::load()
{
    // Default focus
    ui->layoutsListView->setFocus();

    setLayoutsListViewIndex( m_keyboardModel->layout() );
    setVariantsListViewIndex( m_keyboardModel->variant() );
    setModelComboBoxIndex( m_keyboardModel->model() );

    m_keyboardPreview->setLayout( m_keyboardModel->layout() );
    m_keyboardPreview->setVariant( m_keyboardModel->variant() );

    ui->sliderDelay->setValue( m_keyboardModel->delay() );
    ui->sliderRate->setValue( m_keyboardModel->rate() );
}


void
KeyboardModule::setLayoutsListViewIndex( const QString& layout )
{
    QModelIndexList layoutIndexList = m_keyboardProxyModel->match( ui->layoutsListView->rootIndex().child( 0,0 ),
                                      KeyboardModel::KeyRole,
                                      layout,
                                      -1,
                                      Qt::MatchExactly );

    if ( layoutIndexList.size() == 1 )
    {
        QModelIndex layoutIndex = layoutIndexList.first();
        ui->layoutsListView->setCurrentIndex( layoutIndex );
    }
    else
        qDebug() << QString( "Can't find the keyboard layout %1" ).arg( layout );
}


void
KeyboardModule::setVariantsListViewIndex( const QString& variant )
{
    QAbstractItemModel* model = ui->variantsListView->model();
    QModelIndexList variantDefaultList = model->match( model->index( 0,0 ),
                                         KeyboardModel::KeyRole,
                                         variant,
                                         -1,
                                         Qt::MatchExactly );
    if ( variantDefaultList.size() == 1 )
    {
        QModelIndex variantDefault = variantDefaultList.first();
        ui->variantsListView->setCurrentIndex( variantDefault );
        // Emit clicked(), to update keyboardPreview
        emit( ui->variantsListView->clicked( variantDefault ) );
    }
    else
        qDebug() << QString( "Can't find the keyboard variant %1" ).arg( variant );
}


void
KeyboardModule::setModelComboBoxIndex( const QString& model )
{
    QModelIndexList modelIndexList = m_keyboardProxyModel->match( ui->modelComboBox->rootModelIndex().child( 0,0 ),
                                     KeyboardModel::KeyRole,
                                     model,
                                     -1,
                                     Qt::MatchExactly );
    if ( modelIndexList.size() == 1 )
    {
        QModelIndex modelIndex = modelIndexList.first();
        ui->modelComboBox->setCurrentIndex( modelIndex.row() );
    }
    else
        qDebug() << QString( "Can't find the keyboard model %1" ).arg( model );
}

void
KeyboardModule::setNewLayout( const QModelIndex& index )
{
    if ( index.isValid() )
    {
        QString layout = index.data( KeyboardModel::KeyRole ).toString();
        m_keyboardModel->setNewLayout( layout );
        setVariantsListViewIndex( "default" );
    }
}


void
KeyboardModule::setNewVariant( const QModelIndex& index )
{
    if ( index.isValid() )
    {
        QString variant = index.data( KeyboardModel::KeyRole ).toString();
        m_keyboardModel->setNewVariant( variant );
    }
}

#include "KeyboardModule.moc"
