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
# | Filename: email_incomplete_sign_up.pl                                        |
# +------------------------------------------------------------------------------+
# | Description:                                                                 |
# | This perl program will determine if an interview schedule for the next day   |
# | is incomplete and e-mails the contact associated with the job if it is.      |
# | An interview schedule is complete if the first interview time slot for that  |
# | interview on the next day occurs between 23.5 and 24.5 hours from now, and   |
# | the number of free interview time slots in the future (not including the     |
# | interview time slots for the interview schedule one day from now) is less    |
# | than the number of shortlisted students who have not yet signed up for an    |
# | interview.                                                                   |
# +------------------------------------------------------------------------------+

use DBI;

#Set these three variables for access to your database
$database = "";
$user = "";
$password = "";

$dbh = DBI->connect('DBI:mysql:' . $database, $user, $password);

#get the jobs that have the first interview time slot for the interview day 
#that occurs between 23.5 hrs and 24.5 hrs from now 
$sql = "SELECT j.job_code, ec.company_name, i.job_id, min(i.int_time) as min_time, i.int_date
	FROM  job_info as j, interview_time as i, interview as ii, employer_company as ec
	WHERE j.job_id = i.job_id and i.int_date = DATE_ADD(CURRENT_DATE(), INTERVAL 1 DAY)
	AND ii.job_id=j.job_id AND ii.cancelled <> '1' AND ec.employer_id=j.employer_id
	GROUP BY i.job_id, j.job_code
	HAVING time_to_sec(min_time) >= time_to_sec(CURRENT_TIME()) - 1800 AND time_to_sec(min_time) < time_to_sec(CURRENT_TIME()) + 1800";
$sth = $dbh->prepare($sql);
$sth->execute;

#prepare an sql statement to determine the number of unsigned up students
$sql = "SELECT  b.time_id, a.student_number
           FROM            students_shortlisted AS a
           LEFT OUTER JOIN interview_time AS b
           ON                      a.student_number=b.student_number
           AND             b.job_id= ? 
           WHERE           a.job_id= ?
           AND ISNULL(b.time_id)";
$sth_email = $dbh->prepare($sql);


#prepare an sql statement to determine the number of unfilled time slots 
#that occur after the day one day from now.
$sql = "SELECT count(time_id) as unfilled
        FROM interview_time
        WHERE ISNULL(student_number) AND int_type=1 AND 
		int_date > DATE_ADD(CURRENT_DATE(), INTERVAL 1 DAY) 
		and job_id = ?";
$sth_email2 = $dbh->prepare($sql);

#for each job code determine if an e-mail needs to be sent and send it if necessary
while (($job_code, $company_name, $job_id, $start_time, $check_day) = $sth->fetchrow_array){
	if (need_send_email($sth_email, $sth_email2,$job_id)){
		sendEmail($job_id, $job_code, $company_name);
	}
}
$sth_email->finish;
$sth_email2->finish;
$sth->finish;
$dbh->disconnect;


#==========================================================================================================================

#
#sub: sendEmail
#
#Description: Sends an e-mail for the specified job_id to the contact of the job informing them the signup is
#incomplete.
#
sub sendEmail{
	local($job_id, $job_code, $company_name) = @_;
	my $sql;
	my $sth;
	my $body;

	$sql = "SELECT student_number FROM interview_time WHERE job_id = '" . $job_id . "'";
	$sth = $dbh->prepare($sql);
	$sth->execute;
	while (local($student) = $sth->fetchrow_array()){
		$in_str .= "'" . $student . "',";
	}
	chop($in_str);
	$sql = "SELECT DISTINCT CONCAT(s.first_name, ' ', s.last_name) as name, s.student_number
		FROM student as s, students_shortlisted as l
		WHERE s.student_number = l.student_number AND l.job_id = '" . $job_id . "' AND 
			l.student_number NOT IN (" . $in_str . ")";
	$sth = $dbh->prepare($sql);
	$sth->execute;
	while (local($student_name, $student_number) = $sth->fetchrow_array()){
		$body .= $student_number . " : " . $student_name . "\n";
	}

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
	my $subject  = "Subject: Sign-up Incomplete for $job_code ($company_name)";
	my $content  = "The following student(s) have not signed up for $job_code ($company_name):\n" . $body;
	my $from = "From: Mamook";

	open(SENDMAIL, "|$sendmail") || die "Cannot open file: $!";
	print SENDMAIL $reply_to . "\n";
	print SENDMAIL $subject. "\n";
	print SENDMAIL $from . "\n";
	print SENDMAIL $send_to . "\n";
	print SENDMAIL "Content-type: text/plain\n\n";
	print SENDMAIL $content . "\n\n";
	close(SENDMAIL);
}

#
#sub: need_send_email
#
#Description:
#Execute the two prepared sql statements and determine if the number of unsigned up shortlisted students
#is greater than the number of free future time slots (not including the time slots for the day being checked -
#one day from the current day).  Returns 1 if true, 0 otherwise.
#
sub need_send_email{
    	local($sth_email, $sth_email2, $job_id) = @_;
  	local($sql, $sth); 
	
	$sth_email->bind_param(1, $job_id);
	$sth_email->bind_param(2, $job_id);

	$sth_email->execute;
	$num_students = $sth_email->rows;

	$sth_email2->bind_param(1, $job_id);
	
	$sth_email2->execute;
	
	local($num_unfilled_future_slots) = $sth_email2->fetchrow_array;
	
	if ($num_students > $num_unfilled_future_slots){
		return 1;
	}else{
		return 0;
	}
}
