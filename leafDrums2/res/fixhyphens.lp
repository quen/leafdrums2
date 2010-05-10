@files=<*.bmp*>;

foreach $file(@files)
{
if($file =~ /\-/)
 {
   $new=$file;
   $new=~s/\-/\_/g;
   rename($file,$new);
 }
}