#!/usr/bin/perl 

# GPL [OvO]wl, saving time 2013. Part of WitchCraft.

use Cwd 'abs_path';
use File::Basename;
my $Dirname = dirname(__FILE__);
my $abs_path = abs_path($file);

# Only add exempt tests here if likely to fail on a regular basis
my @ExemptTests=(":all.txt",":assimilate.txt",":lastbefore.txt",":lastafter.txt",":firstbefore.txt",":firstafter.txt",":map.txt");

my $FileOne="${abs_path}/${Dirname}/test.txt";
my $FileTwo="${abs_path}/test.txt";
my $TestDir="${abs_path}/Examples/";
my @FileOneData=();
my @FileTwoData=();
my @Tests=();
my @TestDataOne=();
my @TestDataTwo=();
my @FailList=();
my @SegfaultList=();
my @UntestedList=();

my $FailedTests=0;
my $PassedTests=0;
my $Untested=0;

sub CreateTestList();
sub TestFunction($);
sub TestAll();
sub LoadTestOne($);
sub LoadTestTwo($);
sub SummaryScreen();
sub ScanForMissing();

# Warning: invoke LoadTestOne first!
sub ScanForMissing(){
    my ($C,$D,$TestName,$Found);
    my @ExampleTests=`ls -1 "${TestDir}"`;    
    for($C=0;$C<@ExampleTests;$C++){
	$TestName = ${ExampleTests[$C]};
	chomp $TestName;
	$Found=0;
	for($D=0;$D<@Tests;$D++){
	    if( "${Tests[$D]}" =~ /${TestName}/ ){
		$Found=1;
		break;
	    }
	}
	if($Found==0){
	    push(@UntestedList,"${TestName}");
	    $Untested++;
	}
    }
}

sub SummaryScreen(){
    my ($C,$D,$Found,$Exempt);
    my $Total=$FailedTests+$PassedTests;
    my @Segfaults=();

    $Exempt=@ExemptTests;
    $Total+=$Exempt;

    print "\n-[Summary]------\n";
    print "Total: ${Total}, Passed: ${PassedTests}, Failed: ${FailedTests}, Skipped: ${Exempt}, Untested: ${Untested}\n";

    print " \n-[Failed]-------\n";
    for($C=0;$C<@FailList;$C++){
	printf "%20s", "${FailList[$C]}";
	if($C % 3 == 2){
	    print " \n";
	}
    }
    if($C % 3 != 0){
	print " \n";
    }

    if(@UntestedList > 0){
	print " \n-[Untested]-------\n";
	for($C=0;$C<@UntestedList;$C++){
	    printf "%20s", "${UntestedList[$C]}";
	    if($C % 3 == 2){
		print " \n";
	    }
	}
	if($C % 3 != 0){
	    print " \n";
	}
    }

    @Segfaults=`grep -A 1 SEGFAULTS "${FileTwo}" | grep '<PERSON>'`;
    for($C=0;$C<@Segfaults;$C++){
	$Found=0;
	for($D=0;$D<@SegfaultList;$D++){
	    if( "${Segfaults[$C]}" eq "${SegfaultList[$D]}" ){
		$Found=1;
	    }
	}
	if($Found==0){
	    push(@SegfaultList,"${Segfaults[$C]}");
	}
    }

    $D=@SegfaultList;
    if($D){
	print " \n-[Segfaults]----\n";
	for($C=0;$C<@SegfaultList;$C++){
	    print "${Segfaults[$C]}";
	}
    }
    print "\n";
}

sub LoadTestOne($){
    my $TestName=shift;
    my ($Line,$Start,$Chomped);
    @TestDataOne=();
    $Start=0;
    for($Line=0;$Line<@FileOneData;$Line++){
	$Chomped="${FileOneData[$Line]}";
	chomp $Chomped;
	if( "${TestName}" eq "${Chomped}" ){
	    $Start=1;
	}
	else{
	    if($Start==1){
		if("${Chomped}" =~ /<SYSTEM>/ ){
		    return 0;
		}
	    }
	}
	if($Start==1){
	    push(@TestDataOne,"${Chomped}");
	}
    }
}

sub LoadTestTwo($){
    my $TestName=shift;
    my ($Line,$Start,$Chomped);
    @TestDataTwo=();
    $Start=0;
    for($Line=0;$Line<@FileTwoData;$Line++){
	$Chomped="${FileTwoData[$Line]}";
	chomp $Chomped;
	if( "${TestName}" eq "${Chomped}" ){
	    $Start=1;
	}
	else{
	    if($Start==1){
		if("${Chomped}" =~ /<SYSTEM>/ ){
		    return 0;
		}
	    }
	}
	if($Start==1){
	    push(@TestDataTwo,"${Chomped}");
	}
    }
}

sub TestFunction($){
    my $TestName=shift;
    my ($S1,$S2,$C,$Fail);
    LoadTestOne("${TestName}");
    LoadTestTwo("${TestName}");
    $S1=@TestDataOne;
    $S2=@TestDataTwo;
    if($S1 != $S2){
	return 1;
    }
    $Fail=0;
    for($C=0;$C<$S1;$C++){
	if("${TestDataTwo[$C]}" ne "${TestDataOne[$C]}"){
	    if("${TestDataOne[$C]}" =~ m/..:..:..-....\/..\/../){}
	    else{
		$Fail=1;
	    }
	}
    }
    return "${Fail}"
}

sub TestAll(){
    my ($A,$C,$FuncName,$Skip);
    for($A=0;$A<@Tests;$A++){
	chomp ${Tests[${A}]};
	$FuncName="${Tests[${A}]}";
	$FuncName =~ s/^.*\///g;
#	printf "%20s ", "$FuncName";
	$Skip=0;
	for($C=0;$C<@ExemptTests;$C++){
	    if("$FuncName" eq "${ExemptTests[$C]}" ){
		$Skip=1;
	    }
	}
	if($Skip == 0){
	    if(TestFunction("${Tests[$A]}")==1){
#      	    print "     FAIL\n";
		$FailedTests++;
		push(@FailList,"$FuncName");

	    }
	    else{
#	    print "PASS\n";
		$PassedTests++;
	    }
	}
    }    
}

sub CreateTestList(){
    my $A=0;
    @FileOneData=`cat "${FileOne}"`;
    @FileTwoData=`cat "${FileTwo}"`;
    for($A=0;$A<@FileOneData;$A++){
	if("${FileOneData[${A}]}" =~ /<SYSTEM>/ ){
	    push(@Tests,"${FileOneData[${A}]}");
	}
    }
}

CreateTestList();
TestAll();
ScanForMissing();
SummaryScreen();

