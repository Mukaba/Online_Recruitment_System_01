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
# | Filename: email_incomplete_lunch.pl                                          |
# +------------------------------------------------------------------------------+
# | Description:                                                                 |
# | This perl program will determine if a lunch schedule for the next day is     |
# | incomplete and e-mails the contact associated with the interview if it is.   |
# | A lunch schedule is complete if the lunch occurs between 23.5 and 24.5 hours |
# | from now, and no one is signed up for it.                                    |
# +------------------------------------------------------------------------------+

use DBI;

#Set these three variables for access to your database
$database = "";
$user = "";
$password = "";

$dbh = DBI->connect('DBI:mysql:' . $database, $user, $password);

#get the jobs that have the first lunch slot for the interview day occuring between 23.5 hrs 
#and 24.5 hrs from now where the lunch is unassigned. 
$sql = "SELECT j.job_code, e.company_name, i.job_id, min(i.int_time) as min_time, i.int_date
	FROM  job_info as j, interview_time as i, employer_company as e, interview as t
	WHERE j.job_id = i.job_id and i.int_date = DATE_ADD(CURRENT_DATE(), INTERVAL 1 DAY)
	AND i.int_type='3' AND ISNULL(i.student_number) AND e.employer_id=j.employer_id
	AND t.job_id=j.job_id AND t.interview_medium_id='1' AND t.interview_place_id='1'
	AND t.cancelled <> '1'
	GROUP BY i.job_id, j.job_code
	HAVING time_to_sec(min_time) >= time_to_sec(CURRENT_TIME()) - 1800 AND time_to_sec(min_time) < time_to_sec(CURRENT_TIME()) + 1800";
$sth = $dbh->prepare($sql);
$sth->execute;

#for each job code determine if an e-mail needs to be sent and send it if necessary
while (($job_code, $company_name, $job_id, $start_time, $check_day) = $sth->fetchrow_array){
		sendEmail($job_id, $job_code, $company_name);
}
$sth->finish;
$dbh->disconnect;


#==========================================================================================================================

#
#sub: sendEmail
#
#Description: Sends an e-mail for the specified job_id to the contact of the job informing them the lunch signup is
#incomplete.
#
sub sendEmail{
	local($job_id, $job_code, $company_name) = @_;
	my $sql;
	my $sth;
	my $body;

	$sql = "SELECT c.email
		FROM contact as c, interview as i
		WHERE c.contact_id = i.contact and i.job_id = '" . $job_id . "'";
	$sth = $dbh->prepare($sql);
	$sth->execute;
	if ($sth->rows == 0){
		print "error invalid e-mail";
		return;
	}
	local($to) = $sth->fetchrow_array;
	my $send_to = "To: " . $to;
	my $sendmail = "/usr/sbin/sendmail -t";
	my $reply_to = "Reply-to: " . $to;
	my $subject  = "Subject: Lunch unassigned for $job_code ($company_name)";
	my $content  = "Please note that no one has signed up yet for lunch tomorrow with $company_name ($job_code).\n";
	my $from = "From: Mamook";

	open(SENDMAIL, "|$sendmail") || die "Cannot open file: $!";
	print SENDMAIL $reply_to . "\n";
	print SENDMAIL $subject. "\n";
	print SENDMAIL $from . "\n";
	print SENDMAIL $send_bcc . "\n";
	print SENDMAIL $send_to . "\n";
	print SENDMAIL "Content-type: text/plain\n\n";
	print SENDMAIL $content . "\n\n";
	close(SENDMAIL);
}
