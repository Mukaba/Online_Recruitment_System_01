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
# | Filename: email_broken_queries.pl                                            |
# +------------------------------------------------------------------------------+
# | Description:                                                                 |
# | This perl program will e-mail information about broken queries.              |
# +------------------------------------------------------------------------------+

$body = $ARGV[0];
$sql = $ARGV[1];
$brokenquery = $ARGV[2];
$to = $ARGV[3];
$database = $ARGV[4];

#Connecting to the database
use DBI;

#Set these two variables for access to your database
$user = "";
$password = "";

$dbh = DBI->connect('DBI:mysql:' . $database, $user, $password);

$check = "SELECT * FROM broken_queries";
$sth = $dbh->prepare($check);
$sth->execute;

$flag = 0;
while (($query_id, $query, $date_time, $contact_id, $student_number, $ip_address, $counter) = $sth->fetchrow_array){
    #If the broken query is already present in the table the flag is set to 1 and the counter in the table is incremented by 1
    #print("BROKEN    ".$brokenquery."\n\n\n");
    if($query eq $brokenquery){
        $flag = 1;
        $counter = $counter +1;
        $update = "UPDATE broken_queries SET counter = ".$counter." where query_id = ".$query_id; 
        $sth3 = $dbh->prepare($update);
        $sth3->execute;
        $sth3->finish; 
    }
}
$sth->finish;
$dbh->disconnect;


if($flag == 0){
    #The broken query is now added to the INSERT for inserting it into the broken_queries table
    $dbh = DBI->connect('DBI:mysql:' . $database, $user, $password);
    $sth2 = $dbh->prepare($sql);
    $sth2->execute;
        $sth4 = $dbh->prepare("Select LAST_INSERT_ID() as query_id");
        $sth4->execute;
        ($query_id) = $sth4->fetchrow_array;
        $sth4->finish;    
    $sth2->finish;
    $dbh->disconnect;
    sendEmail($query_id);
}


#Now that the query has been added to the db we will now email this
sub sendEmail{
    local($qid) = @_;
    $send_to = "To: " . $to;
    $sendmail = "/usr/sbin/sendmail -t";
    $reply_to = "Reply-to: " . $to;
    $subject  = "Subject: Error in processing query: QID=".$qid;
    $content  = "QUERY_ID: ".$qid."\n\n\n".$body;
    $from = "From: Mamook";
	


	open(SENDMAIL, "|$sendmail") || die "Cannot open file: $!";
	print SENDMAIL $reply_to . "\n";
	print SENDMAIL $subject. "\n";
	print SENDMAIL $from . "\n";
	print SENDMAIL $send_to . "\n";
	print SENDMAIL "Content-type: text/plain\n\n";
	print SENDMAIL $content . "\n\n";
	close(SENDMAIL);
}
