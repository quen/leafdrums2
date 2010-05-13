# CreateSetup.pl
#
# A Perl script that creates leafDrums10.exe from
# /msdev/projects/Setup/release/Setup.exe
# and the specified input files

$SourceFile="../setup/release/Setup.exe";
$DOSSource=$SourceFile;
$DOSSource=~s/\//\\/g;
$TargetFile="\\temp\\leafDrums233.exe";

# Create temporary directory
mkdir("/temp/CreateSetup","rwx");

# Open directory file
open(DIRECTORY,">/temp/CreateSetup/Directory.Setup") || 
	die("Failed to write Directory.Setup");

# Get size of setup .exe
$Size=-s("$SourceFile");
print "\nCopying $DOSSource (size $Size)...\n";

# Set up stats
$Compressed=$Size;
$Uncompressed=$Size;

print DIRECTORY "$Size\t";
$DirectoryLength=length($Size)+1;

# Copy setup .exe to CreateSetup as the start of the
# file we're making
`copy $DOSSource \\temp\\CreateSetup\\file`;

# Delete old target file
`del $TargetFile`;

while(<>)
{
	chop;
	if($_=~/,/)
	{
		# Extract filename and target folder name
		@Temp=split(',',$_);
		$Filename=$Temp[0];
		$Folder=$Temp[1];

		# Get size of uncompressed file
		$Uncompressed+=-s($Filename);

		# Display info to user
                $ShortFilename=$Filename;
                $ShortFilename=~s/^.*\\//;
                while(length($ShortFilename)<30)
                {
                        $ShortFilename.=' ';
                }
                $FixedFolder=$Folder;
                while(length($FixedFolder)<20)
                {
                        $FixedFolder.=' ';
                }

                print "\n$ShortFilename $FixedFolder ";

		# Compress file to /temp/CreateSetup/
		$RealFilename=$Filename;
		`del \\temp\\CreateSetup\\NewBit`;
		`..\\deflate $RealFilename \\temp\\CreateSetup\\NewBit`;
		
		# Get size of file
		$FileSize=-s("/temp/CreateSetup/NewBit");
		print "size $FileSize)...";
		$Compressed+=$FileSize;

		($FileSize>1) || die "File not found or compression failed.";

		# Store info in directory
		$Filename=~s/^.*\\//;
		print DIRECTORY "$Filename\t";
		$DirectoryLength+=length($Filename)+1;
		print DIRECTORY "$Folder\t";
		$DirectoryLength+=length($Folder)+1;
		print DIRECTORY "$FileSize\t";
		$DirectoryLength+=length($FileSize)+1;

		# Add file to the existing "file"
            print "Concat";
            `..\\concat \\temp\\CreateSetup\\file \\temp\\CreateSetup\\NewBit >nul`
	}
}

# Finalise directory
$DirectoryLength+=4;	# Take account of "end" and \t
print DIRECTORY "\t$DirectoryLength"."end";
close DIRECTORY;

# Add directory to end of file
`copy /b \\temp\\CreateSetup\\file+\\temp\\CreateSetup\\Directory.Setup $TargetFile`;

$Ratio=$Uncompressed/$Compressed;
print "\n\nTotal size $Compressed (uncompressed $Uncompressed, ratio $Ratio:1)\n";

