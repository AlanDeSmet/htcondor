Canonical repository for these files is
/afs/cs.wisc.edu/p/condor/workspaces/adesmet/lark/demo-201301 ; which is a
non-bare git repository.  Copies on lark00 or lark01 are just copies.


General flow:

- Web server serves static content from ovswitch:/var/www/html

- The form on the static content issues an asynchronous call to
  ovswitch:/var/www/cgi-bin/set-state.cgi .  You can call this
  with arguments like "a1=3&b2=1"  (3 jobs of type 1 running on
  network Path_A, 1 job of type 2 running on network Path_B.) You
  can make this call from the command line, or from the web
  (http://128.104.59.140/cgi-bin/set-state.cgi?a1=3&b2=1).

- ovswitch's set-state.cgi calls
  ovswitch:/var/www/cgi-bin/set-state .  You can call this with
  arguments like "1 Path_A 3 2 Path_B 1" (3 jobs of type 1
  running on network Path_A, 1 job of type 2 running on network
  Path_B).  ovswitch's set-state requires the ability to ssh to
  larkdemo@lark01 without any password; this is done with a
  passwordless key in ovswitch:~apache/.ssh/id_rsa and the
  appropriate authorized_keys in
  lark01:~larkdemo/.ssh/authorized_keys.

- ovswitch's set-state is a thin wrapper that calls
  larkdemo@lark01:set-state

- lark01's set state uses a variety of HTCondor command line
  tools to check the current state and adjust the number of
  queued jobs appropriately.  If the number of jobs specified is
  0 and there are no jobs in the queue, the DHCP server will be
  restarted to reset the leases; this requires sudo access.  As
  such larkdemo requires sudo access to the DHCP init script as
  well as /bin/rm.

- The jobs themselves upload or download data from the CS
  webserver.  They use sudo to set up their own networking; as
  such larkdemo requires sudo access to various networking
  commands.


Testing

- On the execute node as the larkdemo user, try running "./job
  download http://128.105.7.26/~adesmet/lark/1000000 0 1 100k 0".
  This should spend about 10 seconds downloading a file (into
  /dev/null), then exit successfully.

- On the submit node as the larkdemo user, try "./condor_submit
  1.submit".  This should run a job that runs forever downloading
  files.  It will NOT specify a NetworkPath.  When done,
  condor_rm the job.

- On the submit node as the larkdemo user, try "./set-state 1
  Path_A 1".  This should queue a job that runs forever
  downloading files on the network Path_A.

- On the submit node as the larkdemo user, assuming the previous
  step was successful and the job is still running, try
  "./set-state 1 Path_A 0".  This should remove the job from the
  queue.

- On the graph webserver as the apache user, try "./set-state 1
  Path_A 1" and "./set-state 1 Path_A 0".  Results should be
  similar to the previous two steps.  Note that the job should
  end up queued on the submit node, not the graph webserver.

- On the graph webserver as the apache user, try "./set-state.cgi
  a1=1" and "./set-state.cgi a1=0".  These are functionaly the
  same as the previous pair of commands and the results should be
  the same.

- Access set-state.cgi through the webserver with the commands
  above, replacing the space with a question mark.  For example:
  http://128.104.59.140/cgi-bin/set-state.cgi?a1=1
  http://128.104.59.140/cgi-bin/set-state.cgi?a1=0

- Access the graph page and try out the controls.



If things fail, places to check for clues:

- On the submit node ~larkdemo/out/log is the user log.

- On the submit node ~larkdemo/out/*.out and *.err are standard output and standard error for the individual jobs.

- On the graph webserver, the error_log (Check
  /var/log/httpd/error_log).  Notably, output from set-state on
  the submit machine ends up in this log!

- Condor log files on the submit and execute nodes.



Setting up the download webserver (the server that individual
jobs do uploads or downloads from; currently
http://pages.cs.wisc.edu/~adesmet/lark/)

- You'll need to change the submit files (probably by changing
  create-submit-file and running "./create-submit-file --prefix 1
  1; ./create-submit-file --prefix 2 2")

- You'll need a file to download; currently the submit files
  specify a 1,000,000 byte file. ("dd if=/dev/urandom of=1000000
  bs=1 count=1000000")

- You'll need a CGI to POST uploads to.  Install devnull.pl in
  a location where it can run as a CGI.


Setting up the graph webserver

- Install various files currently on ovswitch:/var/www/html in
  the static HTML location.

- Install contents of ./cgi into the CGI directory.

- Modify set-state.cgi; look for the line "my $SET_STATE =
  '/var/www/cgi-bin/set-state';" and change the path as
  appropriate.

- Install ssh/id_rsa into the .ssh directory as the whichever
  user that webserver runs as (probably apache or http).  Ensure
  the permissions are highly restrictive; SSH gets cranky if
  other users can see the file.

- As the webserver user, ssh to the submit node to populate the
  known_hosts file.


Setting up the submit node

- TODO: Install and set up a DHCP server with a 20 minute lease.
  If the commands to start it, stop it, and to remove its lease
  file are different, you'll need to change ./set-state to use
  the correct commands and change the sudoers permissions below
  to reflect the new commands.

- Create a user larkdemo

- Put the contents of this directory to ~larkdemo. Do so
  recursively.

- Move ssh/authorized_keys2 to ~/.ssh/authorized_keys2.  Ensure
  the permissions are highly restrictive to keep SSH happy.

- Modify /etc/sudoers (probably using visudo) so that the
  larkdemo account doesn't require a TTY.  You'll want a line
  something like:

Defaults:larkdemo !requiretty
Defaults: !visiblepw

- Modify /etc/sudoers so that the larkdemo can reset the DHCP
  service.  It needs to be able to run /etc/rc.d/init.d/dnsmasq
  (with varying arguments) and "/bin/rm
  /var/lib/dnsmasq/dnsmasq.leases".  It needs to run these
  command without a password.  You'll want a line something like:

larkdemo	ALL=(ALL)       NOPASSWD: /bin/rm /var/lib/dnsmasq/dnsmasq.leases,/etc/rc.d/init.d/dnsmasq

  If you're using the same host as the execute node, you'll also
  want access to ifconfig, route, and arpsend, in which case
  you'll instead want:

larkdemo	ALL=(ALL)       NOPASSWD: /sbin/ifconfig,/sbin/route,/usr/local/arpsend/bin/arpsend,/bin/rm /var/lib/dnsmasq/dnsmasq.leases,/etc/rc.d/init.d/dnsmasq



Setting up the execute node (which may be identical to the submit
node)

- TODO: Install arpsend.  If the location is different, you'll
  need to change ./job to use the correct command and change the
  sudoers permissions below to reflect the new commands.

- Modify /etc/sudoers (probably using visudo) so that the
  larkdemo account doesn't require a TTY.  You'll want a line
  something like:

Defaults:larkdemo !requiretty
Defaults: !visiblepw

- Modify /etc/sudoers so that the larkdemo can configure its
  network.  It needs to be able to run ifconfig, route, and
  arpsend.  It needs to run these command without a password.
  You'll want a line something like:

larkdemo	ALL=(ALL)       NOPASSWD: /sbin/ifconfig,/sbin/route,/usr/local/arpsend/bin/arpsend

  If you're using the same host as the submit node, you'll also
  want access to some more commands, in which case you'll instead
  want:

larkdemo	ALL=(ALL)       NOPASSWD: /sbin/ifconfig,/sbin/route,/usr/local/arpsend/bin/arpsend,/bin/rm /var/lib/dnsmasq/dnsmasq.leases,/etc/rc.d/init.d/dnsmasq
