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
# | Filename: interview_status.pl                                                |
# +------------------------------------------------------------------------------+
# | Description:                                                                 |
# | If an interview's last time slot has passed in the last hour then the        |
# | student status for the job gets updated to 'Waiting for Ranking'             |
# |                                                                              |
# | Job Status are as follows:                                                   |
# | 1 - Not Checked                                                              |
# | 2 - Checked                                                                  |
# | 3 - Posted                                                                   |
# | 4 - Closed                                                                   |
# | 5 - Cancelled                                                                |
# | 6 - Rejected                                                                 |
# | 7 - Interview Only                                                           |
# | 8 - Holding                                                                  |
# +------------------------------------------------------------------------------+

use DBI;

#Set these three variables for access to your database
$database = "";
$user = "";
$password = "";

$dbh = DBI->connect('DBI:mysql:' . $database, $user, $password);

#find all interviews whose last interview date and time have passed in the last hour

$sql = "SELECT job_id, MAX(int_date) as date_max, MAX(end_time) as time_max 
	FROM interview_time
	GROUP BY job_id
	HAVING date_max >= DATE_FORMAT(DATE_ADD(SYSDATE(), INTERVAL -1 HOUR), '%Y-%m-%d')
	AND time_max >= DATE_FORMAT(DATE_ADD(SYSDATE(), INTERVAl -1 HOUR), '%H:%i:%S')
	AND date_max <= CURRENT_DATE AND time_max <= CURRENT_TIME";
$sth=$dbh->prepare($sql);
$sth->execute;

while (($job_id, $date, $time) = $sth->fetchrow_array()){
	
	#update student status to waiting for ranking
	$sql = "UPDATE job_info SET student_status = '4' WHERE job_id = '" . $job_id . "' AND student_status='3'";
	$dbh->do($sql);

	#update admin status to waiting for ranking
	$sql = "UPDATE job_info SET admin_status='17' WHERE job_id = '" . $job_id . "'";
	$dbh->do($sql);

	$sql = "SELECT job_id FROM interview_job_join WHERE interview_job_id='".$job_id."'";
	$tth=$dbh->prepare($sql);
	$tth->execute;
	while (($job_id) = $tth->fetchrow_array()){
		#update student status to waiting for ranking for multiple jobs with one interview
		$sql = "UPDATE job_info SET student_status = '4' WHERE job_id = '" . $job_id . "' AND student_status='3'";
		$dbh->do($sql);

		#update admin status to waiting for ranking for multiple jobs with one interview
		$sql = "UPDATE job_info SET admin_status='17' WHERE job_id = '" . $job_id . "'";
		$dbh->do($sql);
	}
	$tth->finish;
	
}

$sth->finish;

$dbh->disconnect;
 
