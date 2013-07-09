#! /usr/bin/perl
use CGI;

# Paranoia
$CGI::POST_MAX = 1024*8; # 8k of input should be plenty
$CGI::DISABLE_UPLOADS = 1;

my $SET_STATE = '/var/www/cgi-bin/set-state';


# We call out to other programs that may be noisy. So create a new
# handle for CGI output and route normal standard output to standard
# error.
open(CGIOUT, ">&STDOUT") or mydie("$0: Failed to dup(STDOUT): $!");
open(STDOUT, ">&STDERR") or mydie("$0: Failed to dup(STDERR): $!");

my $query = CGI->new();

my @args = ($SET_STATE);
push @args, args_for_set_state(1, 'Path_A', $query, 'a1');
push @args, args_for_set_state(2, 'Path_A', $query, 'a2');
push @args, args_for_set_state(1, 'Path_B', $query, 'b1');
push @args, args_for_set_state(2, 'Path_B', $query, 'b2');

#mylog("@args");
my $ret = system(@args);

print CGIOUT "Content-type: text/plain\n\n";
if($ret != 0) {
		mylog("Error: set-state returned $ret");
		print CGIOUT "1\nError\nset-state returned $ret\n";
} else {
		print CGIOUT "0\nOK\n";
}

sub args_for_set_state {
	my($job, $path, $query, $param) = @_;
	my $count = $query->param($param);
	if(not defined $count or $count !~ /^\d+$/) { return; }
	return ($job, $path, $count);
}

sub mydie {
		mylog(@_);
		die(@_);
}

sub mylog {
		($sec,$min,$hour,$mday,$mon,$year) = localtime(time);
		print STDERR sprintf("%04d-%02d-%02d %02d:%02d:%02d %s (%d) ", $year+1900, $mon, $mday +1, $hour, $min, $sec, $0, $$), @_, "\n";

}
