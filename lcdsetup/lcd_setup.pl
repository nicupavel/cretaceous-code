#!/usr/bin/perl -w
# npavel@ituner.com - MtxOrb driver functions

use Fcntl;
use POSIX;
use IO; 
#-------------------------------------------------------------------------------
my $LCD_PORT = "/dev/ttyS1";
my $FATAL_ERROR = "Could not open '$LCD_PORT' LCD device";
my $KB_AUTO_REPEAT_OFF	     = 0;
my $KB_AUTO_REPEAT_TYPEMATIC = 1;
my $KB_AUTO_REPEAT_CODE      = 2;
#
sub InitLCD
{
    sysopen(LCD, "$LCD_PORT", O_NONBLOCK|O_RDWR) || die $FATAL_ERROR;
    my $fd_disp = fileno ( LCD );
    my $termios = POSIX::Termios->new ( );
    $termios->getattr ( $fd_disp );
    $termios->setispeed ( B19200 );
    $termios->setospeed ( B19200 );
    my $cflag = $termios->getcflag ( );
    my $lflag = $termios->getlflag ( );
    my $oflag = $termios->getoflag ( );
    my $iflag = $termios->getiflag ( );
    $iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON);
    $oflag &= ~OPOST;
    $lflag &= ~(ECHO|ECHONL|ICANON|ISIG);
    $cflag &= ~(CSIZE|PARENB|HUPCL);
    $cflag |= CS8|CLOCAL;
    $termios->setcflag ( $cflag );
    $termios->setlflag ( $lflag );
    $termios->setoflag ( $oflag );
    $termios->setiflag ( $iflag );
    $termios->setattr ( $fd_disp, TCSANOW );
}
#-------------------------------------------------------------------------------
sub CloseLCD {
    close (LCD);
}
#-------------------------------------------------------------------------------
sub SetBacklight
{
  my $Backlight = shift;
  printf LCD $Backlight ? "\xFE\x42\x00" : "\xFE\x46";
}
#-------------------------------------------------------------------------------
sub SetContrast
{
  my $Contrast = shift;
  
  printf LCD "\xFE\x50%c\xFE\x91", $Contrast;
}
#-------------------------------------------------------------------------------
sub GPOOn
{
    my $GPONumber = shift;
    printf LCD "\xFE\x57%c", $GPONumber;
    
}
sub GPOOff
{
    my $GPONumber = shift;
    printf LCD "\xFE\x56%c", $GPONumber;
    
}
#-------------------------------------------------------------------------------
sub SetAutoRepeat
{
  my $AutoRepeatMode = shift;
  
  if ($KB_AUTO_REPEAT_OFF == $AutoRepeatMode) {
    printf LCD "\xFE\x60";
  }
  else {
    printf LCD "\xFE\x7E%c", $KB_AUTO_REPEAT_TYPEMATIC == $AutoRepeatMode 
      ? 0x00 : 0x01;
  }
}
#-------------------------------------------------------------------------------
sub SetAutoLineWrap
{
  my $AutoLineWrap = shift;
  
  printf LCD "\xFE%c", $AutoLineWrap ? 0x43 : 0x44;
}
#-------------------------------------------------------------------------------
sub SetAutoScroll
{
  my $AutoScroll = shift;
  
  printf LCD "\xFE%c", $AutoScroll ? 0x51 : 0x52;
}
#-------------------------------------------------------------------------------
sub SetAutoTransmitKeypresses
{
  my $AutoTransmitKeypresses = shift;
  
  open(LCD, ">$LCD_PORT") || die $FATAL_ERROR;
  printf LCD "\xFE%c", $AutoTransmitKeypresses ? 0x41 : 0x4F;
}
#-------------------------------------------------------------------------------
sub PollKeypad
{
  printf LCD "\xFE\x26";
  my $KeyCode = getc(LCD);
  
  return $KeyCode;
}
#-------------------------------------------------------------------------------
sub ClearDisplay
{
  printf LCD "\xFE\x58";
}
#-------------------------------------------------------------------------------
sub ShowCursor
{
  my $Cursor = shift;

  printf LCD "\xFE%c", $Cursor ? 0x4A : 0x4B;
}
#-------------------------------------------------------------------------------
sub ChangeBootLogo
{
# 40 chars can be send as a boot logo 0-20 first row 21-40 the second row
#             0                 20|21                 |40       
#             01234567890123456789012345678901234567890     
  my $Logo = "MediaBox 2600 Series(c) 2002 Ituner Corp";
  
  printf LCD "\xFE\x40%s",$Logo;
}
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Operations should be defined here

&InitLCD();
&ClearDisplay();
#&SetBacklight(1);
#&SetContrast(50);
#&SetAutoRepeat($KB_AUTO_REPEAT_TYPEMATIC);
#&SetAutoLineWrap(1);
#&SetAutoScroll(1);
#&SetAutoTransmitKeypresses(0);
#&ShowCursor(0);

#for (; ;) {
#  print &PollKeypad();
#  sleep 1;
#}

#&ChangeBootLogo();
#&GPOOff(1);
#&SetBacklight(1);
&CloseLCD();
