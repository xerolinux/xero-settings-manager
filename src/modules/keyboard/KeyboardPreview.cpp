/*
 *  This file is part of XeroLinux Settings Manager.
 *
 *  Roland Singer <roland@manjaro.org>
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

#include "KeyboardPreview.h"

KeyBoardPreview::KeyBoardPreview( QWidget* parent )
    : QWidget( parent )
    , space( 0 )
    , usable_width( 0 )
    , key_w( 0 )
{
    setMaximumSize( 700, 190 );
    setMinimumSize( 600, 165 );
    layout = "us";

    lowerFont = QFont( "Helvetica", 10, QFont::DemiBold );
    upperFont = QFont( "Helvetica", 8 );

    // Setup keyboard types
    kbList[KB_104].kb_extended_return = false;
    kbList[KB_104].keys.append( QList<int>() << 0x29 << 0x2 << 0x3 << 0x4 << 0x5 << 0x6 << 0x7 << 0x8 << 0x9 << 0xa << 0xb << 0xc << 0xd );
    kbList[KB_104].keys.append( QList<int>() << 0x10 << 0x11 << 0x12 << 0x13 << 0x14 << 0x15 << 0x16 << 0x17 << 0x18 << 0x19 << 0x1a << 0x1b << 0x2b );
    kbList[KB_104].keys.append( QList<int>() << 0x1e << 0x1f << 0x20 << 0x21 << 0x22 << 0x23 << 0x24 << 0x25 << 0x26 << 0x27 << 0x28 );
    kbList[KB_104].keys.append( QList<int>() << 0x2c << 0x2d << 0x2e << 0x2f << 0x30 << 0x31 << 0x32 << 0x33 << 0x34 << 0x35 );

    kbList[KB_105].kb_extended_return = true;
    kbList[KB_105].keys.append( QList<int>() << 0x29 << 0x2 << 0x3 << 0x4 << 0x5 << 0x6 << 0x7 << 0x8 << 0x9 << 0xa << 0xb << 0xc << 0xd );
    kbList[KB_105].keys.append( QList<int>() << 0x10 << 0x11 << 0x12 << 0x13 << 0x14 << 0x15 << 0x16 << 0x17 << 0x18 << 0x19 << 0x1a << 0x1b );
    kbList[KB_105].keys.append( QList<int>() << 0x1e << 0x1f << 0x20 << 0x21 << 0x22 << 0x23 << 0x24 << 0x25 << 0x26 << 0x27 << 0x28 << 0x2b );
    kbList[KB_105].keys.append( QList<int>() << 0x54 << 0x2c << 0x2d << 0x2e << 0x2f << 0x30 << 0x31 << 0x32 << 0x33 << 0x34 << 0x35 );

    kbList[KB_106].kb_extended_return = true;
    kbList[KB_106].keys.append( QList<int>() << 0x29 << 0x2 << 0x3 << 0x4 << 0x5 << 0x6 << 0x7 << 0x8 << 0x9 << 0xa << 0xb << 0xc << 0xd << 0xe );
    kbList[KB_106].keys.append( QList<int>() << 0x10 << 0x11 << 0x12 << 0x13 << 0x14 << 0x15 << 0x16 << 0x17 << 0x18 << 0x19 << 0x1a << 0x1b );
    kbList[KB_106].keys.append( QList<int>() << 0x1e << 0x1f << 0x20 << 0x21 << 0x22 << 0x23 << 0x24 << 0x25 << 0x26 << 0x27 << 0x28 << 0x29 );
    kbList[KB_106].keys.append( QList<int>() << 0x2c << 0x2d << 0x2e << 0x2f << 0x30 << 0x31 << 0x32 << 0x33 << 0x34 << 0x35 << 0x36 );

    kb = &kbList[KB_104];
}


void
KeyBoardPreview::setLayout( const QString layout )
{
    this->layout = layout;
}


void
KeyBoardPreview::setVariant( const QString variant )
{
    this->variant = variant;

    if ( !loadCodes() )
        return;

    loadInfo();
    repaint();
}


void
KeyBoardPreview::loadInfo()
{
    if ( layout == "us" || layout == "th" )
    {
        // kb_104
        kb = &kbList[KB_104];
    }
    else if ( layout == "jp" )
    {
        // kb_106
        kb = &kbList[KB_106];
    }
    else
    {
        // most keyboards are 105 key so default to that
        kb = &kbList[KB_105];
    }
}


bool
KeyBoardPreview::loadCodes()
{
    if ( layout.isEmpty() )
        return false;

    QStringList param;
    param << "-model" << "pc106" << "-layout" << layout << "-compact";
    if ( !variant.isEmpty() )
        param << "-variant" << variant;

    QProcess process;
    process.setEnvironment( QStringList() << "LANG=C" << "LC_MESSAGES=C" );
    process.start( "ckbcomp", param );
    if ( !process.waitForStarted() )
        return false;

    if ( !process.waitForFinished() )
        return false;

    // Clear codes
    codes.clear();

    QStringList list = QString( process.readAll() ).split( "\n", QString::SkipEmptyParts );

    foreach ( QString line, list )
    {
        if ( !line.startsWith( "keycode" ) || !line.contains( '=' ) )
            continue;

        QStringList split = line.split( '=' ).at( 1 ).trimmed().split( ' ' );
        if ( split.size() < 4 )
            continue;

        Code code;
        code.plain = fromUnicodeString( split.at( 0 ) );
        code.shift = fromUnicodeString( split.at( 1 ) );
        code.ctrl = fromUnicodeString( split.at( 2 ) );
        code.alt = fromUnicodeString( split.at( 3 ) );

        if ( code.ctrl == code.plain )
            code.ctrl = "";

        if ( code.alt == code.plain )
            code.alt = "";

        codes.append( code );
    }

    return true;
}


QString
KeyBoardPreview::fromUnicodeString( const QString raw ) const
{
    if ( raw.startsWith( "U+" ) )
        return QChar( raw.mid( 2 ).toInt( 0, 16 ) );
    else if ( raw.startsWith( "+U" ) )
        return QChar( raw.mid( 3 ).toInt( 0, 16 ) );

    return "";
}


QString
KeyBoardPreview::regular_text( int index ) const
{
    if ( index < 0 || index >= codes.size() )
        return "";

    return codes.at( index - 1 ).plain;
}


QString
KeyBoardPreview::shift_text( int index ) const
{
    if ( index < 0 || index >= codes.size() )
        return "";

    return codes.at( index - 1 ).shift;
}


QString
KeyBoardPreview::ctrl_text( int index ) const
{
    if ( index < 0 || index >= codes.size() )
        return "";

    return codes.at( index - 1 ).ctrl;
}


QString
KeyBoardPreview::alt_text( int index ) const
{
    if ( index < 0 || index >= codes.size() )
        return "";

    return codes.at( index - 1 ).alt;
}


void
KeyBoardPreview::resizeEvent( QResizeEvent* )
{
    space = 6;
    usable_width = width() - 6;
    key_w = ( usable_width - ( 14 * space ) )/15;

    setMaximumHeight( ( key_w * 4 ) + ( space * 5 ) );
}


void
KeyBoardPreview::paintEvent( QPaintEvent* event )
{
    QPainter p( this );
    p.setRenderHint( QPainter::Antialiasing );

    p.setBrush( QColor( 0xd6, 0xd6, 0xd6 ) );
    p.drawRect( rect() );

    QPen pen;
    pen.setWidth( 1 );
    pen.setColor( QColor( 0x58, 0x58, 0x58 ) );
    p.setPen( pen );

    p.setBrush( QColor( 0x58, 0x58, 0x58 ) );

    p.setBackgroundMode( Qt::TransparentMode );

    double rx = 3;
    double x = 6;
    double y = 6;
    double first_key_w = 0;
    double remaining_x[] = {0,0,0,0};
    double remaining_widths[] = {0,0,0,0};

    for ( int i = 0; i < 4; i++ )
    {
        if ( first_key_w > 0 )
        {
            first_key_w = first_key_w * 1.375;

            if ( kb == &kbList[KB_105] && i == 3 )
                first_key_w = key_w * 1.275;

            p.drawRoundedRect( QRectF( 6, y, first_key_w, key_w ), rx, rx );
            x = 6 + first_key_w + space;
        }
        else
            first_key_w = key_w;



        bool last_end = ( i == 1 && !kb->kb_extended_return );
        double rw = usable_width - x;
        int ii = 0;

        foreach ( int k, kb->keys.at( i ) )
        {
            QRectF rect = QRectF( x, y, key_w, key_w );

            if ( ii == kb->keys.at( i ).size()-1 && last_end )
                rect.setWidth( rw );

            p.drawRoundedRect( rect, rx, rx );

            rect.adjust( 5, 1, 0, 0 );

            p.setPen( QColor( 0x9e, 0xde, 0x00 ) );
            p.setFont( upperFont );
            p.drawText( rect, Qt::AlignLeft | Qt::AlignTop, shift_text( k ) );

            rect.setBottom( rect.bottom() - 2.5 );

            p.setPen( QColor( 0xff, 0xff, 0xff ) );
            p.setFont( lowerFont );
            p.drawText( rect, Qt::AlignLeft | Qt::AlignBottom, regular_text( k ) );

            rw = rw - space - key_w;
            x = x + space + key_w;
            ii = ii + 1;

            p.setPen( pen );
        }



        remaining_x[i] = x;
        remaining_widths[i] = rw;

        if ( i != 1 && i != 2 )
            p.drawRoundedRect( QRectF( x, y, rw, key_w ), rx, rx );

        x = .5;
        y = y + space + key_w;
    }


    if ( kb->kb_extended_return )
    {
        rx = rx * 2;
        double x1 = remaining_x[1];
        double y1 = 6 + ( key_w * 1 ) + ( space * 1 );
        double w1 = remaining_widths[1];
        double x2 = remaining_x[2];
        double y2 = 6 + ( key_w * 2 ) + ( space * 2 );

        // this is some serious crap... but it has to be so
        // maybe one day keyboards won't look like this...
        // one can only hope
        QPainterPath pp;
        pp.moveTo( x1, y1 + rx );
        pp.arcTo( x1, y1, rx, rx, 180, -90 );
        pp.lineTo( x1 + w1 - rx, y1 );
        pp.arcTo( x1 + w1 - rx, y1, rx, rx, 90, -90 );
        pp.lineTo( x1 + w1, y2 + key_w - rx );
        pp.arcTo( x1 + w1-rx, y2 + key_w - rx, rx, rx, 0, -90 );
        pp.lineTo( x2 + rx, y2 + key_w );
        pp.arcTo( x2, y2 + key_w - rx, rx, rx, -90, -90 );
        pp.lineTo( x2, y1 + key_w );
        pp.lineTo( x1 + rx, y1 + key_w );
        pp.arcTo( x1, y1 + key_w - rx, rx, rx, -90, -90 );
        pp.closeSubpath();

        p.drawPath( pp );
    }
    else
    {
        x = remaining_x[2];
        y = 6 + ( key_w * 2 ) + ( space * 2 );
        p.drawRoundedRect( QRectF( x, y, remaining_widths[2], key_w ), rx, rx );
    }


    QWidget::paintEvent( event );
}
