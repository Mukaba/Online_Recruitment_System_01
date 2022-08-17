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
# | Filename: job_status.pl                                                      |
# +------------------------------------------------------------------------------+
# | Description:  This script does two things:                                   |
# | 1. If a job's closing time has occurred in the last time increment then the  |
# | status gets updated to 'CLOSED' and the student status gets updated to       |
# | 'No Longer Accepting Applications'.                                          |
# | 2. Posts jobs after the 24 hr holding pattern                                |
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

#find all jobs that have closed in the last time increment 

$sql = "SELECT job_id 
	FROM job_info
	WHERE status='3'
	AND closing_date <= CURRENT_DATE()
	AND closing_time <= CURRENT_TIME()";

$sth = $dbh->prepare($sql);
$sth->execute;


while (($job_id) = $sth->fetchrow_array()){
	
	#update status - closed
	$sql = "UPDATE job_info SET status = '4', admin_status='12' where job_id = '" . $job_id . "'";
	$dbh->do($sql);

	#update student status to 'No longer Accepting Applications'
	$sql = "UPDATE job_info SET student_status = '2' where job_id = '" . $job_id . "' AND student_status='1'";
	$dbh->do($sql);
}
$sth->finish;

#Post all of the jobs that have been in the holding pattern for >= 24 hours.
#Discount jobs that were put in the pattern on Fri, Sat or Sun.

$sql = "
	UPDATE job_info
	SET status='3',
	student_status='1',
	admin_status='11',
	date_posted=CURRENT_DATE()
	WHERE hold_end_datetime <= NOW()
	AND status='8'
	AND admin_status='25'
	";
$sth = $dbh->prepare($sql);
$sth->execute();

$dbh->disconnect;
 
