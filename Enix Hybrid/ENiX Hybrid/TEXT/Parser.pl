#!/usr/bin/perl -w 

#use strict;

sub Parse{
    my @Data=`cat identities.txt`;
    my @Identities=();
    my $A;
    my $B;
    my $C;
    my $IdFound;
    for($A=0;$A<@Data-2;$A++){
	if($Data[$A] =~ /^Identities.*$/){
	    if($Data[${A}+2] =~ /^.+$/){
#		print "${Data[$A]}";
		$IdFound=0;
		for($C=0;$C<@Identities;$C++){
		    if( "${Data[$A]}" eq "${Identities[$C]}"){
			$IdFound=1;
		    }
		    
		}
#		print "Found: ${IdFound} \n";
		if($IdFound==0){
		    push @Identities,"$Data[$A]";
		    for($B=$A;$B<@Data;$B++){
			if($Data[$B] =~ /^$/){
			    $B=@Data;
			    print "\n";
			}
			else{
			    print "${Data[$B]}";
			}
		    }
		}
	    }
	}
    }

}

Parse;
