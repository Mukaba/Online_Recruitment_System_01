#!/usr/bin/perl
# +------------------------------------------------------------------------------+
# | Mamook(R) Software                                                           |
# +------------------------------------------------------------------------------+
# | Copyright (c) 2000-2005 University of Victoria.  All rights reserved.        |
# +------------------------------------------------------------------------------+
# | THE LICENSED WORK IS PROVIDED UNDER THE TERMS OF THE ADAPTIVE PUBLIC LICENSE |
# | ("LICENSE") AS FIRST COMPLETED BY: The University of Victoria. ANY USE,      |
# | PUBLIC DISPLAY, PUBLIC PERFORMANCE, REPRODUCTION OR DISTRIBUTION OF, OR      |
# | PREPARATION OF DERIVATIVE WORKS BASED ON, THE LICENSED WORK CONSTITUTES      |
# | RECIPIENT'S ACCEPTANCE OF THIS LICENSE AND ITS TERMS, WHETHER OR NOT SUCH    |
# | RECIPIENT READS THE TERMS OF THE LICENSE. "LICENSED WORK" AND "RECIPIENT"    |
# | ARE DEFINED IN THE LICENSE. A COPY OF THE LICENSE IS LOCATED IN THE TEXT     |
# | FILE ENTITLED "LICENSE.TXT" ACCOMPANYING THE CONTENTS OF THIS FILE. IF A     |
# | COPY OF THE LICENSE DOES NOT ACCOMPANY THIS FILE, A COPY OF THE LICENSE MAY  |
# | ALSO BE OBTAINED AT THE FOLLOWING WEB SITE: http://www.mamook.net            |  
# |                                                                              |
# | Software distributed under the License is distributed on an "AS IS" basis,   |
# | WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for |
# | the specific language governing rights and limitations under the License.    | 
# +------------------------------------------------------------------------------+
# | Filename: email_signup_complete.pl                                           |
# +------------------------------------------------------------------------------+
# | Description:                                                                 |
# | This file determines if an interview signup is complete and e-mails the      |
# | contact associated with the job when that happens.  Lastly, it updates       |
# | the emailed_secretary flag in the interview table so that the secretary      |
# | won't get e-mailed again.                                                    |
# +------------------------------------------------------------------------------+


use DBI;

#Set these three variables for access to your database
$database = "";
$user = "";
$password = "";

$dbh = DBI->connect('DBI:mysql:' . $database, $user, $password);

#Prepares a statement that checks if the signup is complete
$sql = "SELECT  b.time_id, a.student_number
              FROM            students_shortlisted AS a
               LEFT OUTER JOIN interview_time AS b
               ON                      a.student_number=b.student_number
               AND             b.job_id = ?
               WHERE           a.job_id = ? AND
                                       ISNULL(b.time_id)";
$sth_signup = $dbh->prepare($sql);

#prepare sql to check if secretary has been e-mailed
$sql = "SELECT emailed_secretary 
	FROM interview
	WHERE job_id = ?";
$sth_sec = $dbh->prepare($sql);

#get all non-cancelled interviews that occur in the future
$sql = "SELECT DISTINCT j.job_id, j.job_code, ec.company_name
	FROM interview as ii, employer_company as ec, job_info as j LEFT JOIN interview_time as t ON j.job_id = t.job_id 
	WHERE ec.employer_id=j.employer_id AND ii.job_id = j.job_id AND ii.cancelled <> '1' AND t.int_date > CURRENT_DATE()";
$sth = $dbh->prepare($sql);
$sth->execute;

#check if interview signup complete and secretary has not been e-mailed; then send an e-mail and update the flag
while (($job_id, $job_code, $company_name) = $sth->fetchrow_array){
	if (isSignUpComplete($sth_signup, $job_id) && !hasSecretaryEmailed($sth_sec, $job_id)){
		sendEmail($job_id, $job_code, $company_name);
	}
}

$sth->finish;
$sth_signup->finish;
$sth_sec->finish;
$dbh->disconnect;

#=====================================================================================================================

#
#sub: sendEmail
#Parameters: $job_id, $job_code - the job_id and corresponding job code for the interview whose signup is complete.
#Description: Sends an e-mail to the contact for the interview informing them that the interview signup is complete.  Once
#the e-mail has been sent the emailed_secretary flag is updated.
#
sub sendEmail{
	local($job_id, $job_code, $company_name) = @_;
	my $sql;
	my $sth;
	$sql = "SELECT c.email
		FROM contact as c, interview as i
		WHERE c.contact_id = i.contact and i.job_id = '" . $job_id . "'";
	$sth = $dbh->prepare($sql);
	$sth->execute;
	if ($sth->rows == 0){
		print "Error invalid e-mail";
		return;
	}
	local($to) = $sth->fetchrow_array;
	
	my $send_to = "To: " . $to;
	my $sendmail = "/usr/sbin/sendmail -t";
	my $reply_to = "Reply-to: $to";
	my $subject  = "Subject: Sign-up complete for $job_code ($company_name)";
	my $content  = "The sign-up is now complete for $job_code ($company_name)";


	open(SENDMAIL, "|$sendmail") || die "Cannot open file: $!";
	print SENDMAIL $reply_to . "\n";
	print SENDMAIL $subject. "\n";
	print SENDMAIL $send_to . "\n";
	print SENDMAIL "From: Mamook" . "\n";
	print SENDMAIL "Content-type: text/plain\n\n";
	print SENDMAIL $content . "\n\n";
	close(SENDMAIL);

	$sql = "UPDATE interview SET emailed_secretary = 1 WHERE job_id = '" . $job_id . "'";
	$dbh->do($sql);
	
}
#
#sub: isSignUpComplete
#parameters: $sth_signup - the prepared sql statement used to determine if the schedule is complete
#	     $job_id - the job id you want to check
#
#Description: Determines if the interview signup for the specified job_id is complete.
#
sub isSignUpComplete{
    	local($sth_signup, $job_id) = @_;
  	local($sql); 
	$sth_signup->bind_param(1, $job_id);
	$sth_signup->bind_param(2, $job_id);
	$sth_signup->execute;
	if ($sth_signup->rows == 0){
		return 1;
	}else{
		return 0;
	}	
}
#
#sub: hasSecretaryEmailed 
#parameters: $sth_sec - the prepared sql statement used to determine if the secretary has been e-mailed 
#	     $job_id - the job id you want to check
#
#Description: Determines if the secretary has been e-mailed

sub hasSecretaryEmailed{
	local($sth_sec, $job_id) = @_;
	
	$sth_sec->bind_param(1, $job_id);
	$sth_sec->execute;
	
	($flag) = $sth_sec->fetchrow_array();
	if ($flag == 0){
		return 0;
	}else{
		return 1;
	}
}
