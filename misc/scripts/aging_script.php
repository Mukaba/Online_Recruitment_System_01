#!/usr/local/bin/php
<?php

/*

 +------------------------------------------------------------------------------+
 | Mamook(R) Software                                                           |
 +------------------------------------------------------------------------------+
 | Copyright (c) 2000-2005 University of Victoria.  All rights reserved.        |
 +------------------------------------------------------------------------------+
 | THE LICENSED WORK IS PROVIDED UNDER THE TERMS OF THE ADAPTIVE PUBLIC LICENSE |
 | ("LICENSE") AS FIRST COMPLETED BY: The University of Victoria. ANY USE,      |
 | PUBLIC DISPLAY, PUBLIC PERFORMANCE, REPRODUCTION OR DISTRIBUTION OF, OR      |
 | PREPARATION OF DERIVATIVE WORKS BASED ON, THE LICENSED WORK CONSTITUTES      |
 | RECIPIENT'S ACCEPTANCE OF THIS LICENSE AND ITS TERMS, WHETHER OR NOT SUCH    |
 | RECIPIENT READS THE TERMS OF THE LICENSE. "LICENSED WORK" AND "RECIPIENT"    |
 | ARE DEFINED IN THE LICENSE. A COPY OF THE LICENSE IS LOCATED IN THE TEXT     |
 | FILE ENTITLED "LICENSE.TXT" ACCOMPANYING THE CONTENTS OF THIS FILE. IF A     |
 | COPY OF THE LICENSE DOES NOT ACCOMPANY THIS FILE, A COPY OF THE LICENSE MAY  |
 | ALSO BE OBTAINED AT THE FOLLOWING WEB SITE: http://www.mamook.net            |  
 |                                                                              |
 | Software distributed under the License is distributed on an "AS IS" basis,   |
 | WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for |
 | the specific language governing rights and limitations under the License.    | 
 +------------------------------------------------------------------------------+
 | Filename: aging_script.php                                                   |
 +------------------------------------------------------------------------------+
 | Description: This script is invoked by a cron job. It handles the contact    |
 |              statuses. It changes their status based on time.                |
 +------------------------------------------------------------------------------+

 Note: This file must be run from misc/scripts directory

*/

$ENABLE_SQL = 1;
$DEBUG_ON = 0;

include('../release_constants.inc'); // release_constants to be included before constants.inc
include('../constants.inc');  // constants must be included before wwwclient.inc                                                                   
include('../names.inc');                                                                                                                           
include('../functions.inc');                                                                                                                       
include('../wwwclient.inc');                                                                                                                       
include(PATH_ADODB . 'adodb.inc.php');
include('../db.inc');                                                                                                                              
include('../resume_functions.inc');                                                                                                                
include('../special_flags_functions.inc');                                                                                                         

$begin_time = time();
if ($_SERVER['SERVER_ADDR'] != $_SERVER['REMOTE_ADDR'] && !in_array($_SERVER['REMOTE_ADDR'],$IPS_FOR_DEBUG))
{
    exit("Security Violation");
}
else
{
    if ($DEBUG_ON)
    {
        print "Debugging Active<br />";
    }

    // cycle through each department
    // go through contacts where current_status != CONTACT_POTENTIAL_LEAD
    // if a contact's status has changed, get the division_id and employer_id to update division/employer's status. 

    $current_date = date("Y-m-d");
    // For UVIC, the grace period is 4 months
    $grace_period_string = ("-".GRACE_PERIOD." Month");
    $time_stamp = strtotime($grace_period_string, strtotime($current_date));
    $one_grace_period_ago = date("Y-m-d",$time_stamp);

    $grace_period_string = ("-". 3*GRACE_PERIOD." Month");
    $time_stamp = strtotime($grace_period_string, strtotime($current_date));
    $three_grace_period_ago = date("Y-m-d",$time_stamp);

    $dept_sql = ("
        SELECT department_id
        FROM department
        WHERE job_list = '1'
        ");
    $dept_result = $GLOBALS['dbh']->Execute($dept_sql);

    // counters for results
    $inactive_veteran_to_inactive_lapsed = 0;
    $active_to_inactive_veteran = 0;
    $active_new_to_active_veteran = 0;
    $active_lapsed_to_active_veteran = 0;

    $arr_inactive_veteran_to_inactive_lapsed = array();
    $arr_active_to_inactive_veteran = array();
    $arr_active_new_to_active_veteran = array();
    $arr_active_lapsed_to_active_veteran = array();

    while ($dept_row = $dept_result->FetchRow())
    {
        $department_id = $dept_row['department_id'];

        // Any inactive veteran status degrading down to inactive lapsed
        $contact_status_sql = ("
            SELECT dcs.contact_id, DATE_ADD(dcs.activity_date, INTERVAL ". 3 * GRACE_PERIOD." MONTH) AS activity_date, dcs.status_id, ce.department_id, ce.employer_id
            FROM department_contact_status dcs
            INNER JOIN contact_employer ce
            ON dcs.contact_id = ce.contact_id
            WHERE dcs.current_status = '1' AND dcs.status_id = '".CONTACT_INACTIVE_VETERAN."'
            AND dcs.department_id = '".$department_id."' AND dcs.activity_date <= '".$three_grace_period_ago."'
            ");
        $contact_status_result = $GLOBALS['dbh']->Execute($contact_status_sql);

        while($contact_status_row = $contact_status_result->FetchRow())
        {
            $contact_id = $contact_status_row['contact_id'];
            $division_id = $contact_status_row['department_id'];
            $employer_id = $contact_status_row['employer_id'];
            $activity_date = $contact_status_row['activity_date'];

            $inactive_lapsed_sql = ("
                INSERT INTO department_contact_status
                (status_id, contact_id, department_id, entered_on, activity_date, activity_type_id)
                VALUES
                ('".CONTACT_INACTIVE_LAPSED."', '".$contact_id."','".$department_id."', NOW(), '".$activity_date."', '".AUTOMATED."')
                ");
            $inactive_veteran_to_inactive_lapsed++;
            $arr_inactive_veteran_to_inactive_lapsed[] = $contact_id." ".$department_id;
            if ($DEBUG_ON)
            {
                print $inactive_lapsed_sql."<br />";
            } 

            if ($ENABLE_SQL)
            {
                $GLOBALS['dbh']->Execute($inactive_lapsed_sql);

                // Set the latest contact status to be the current status. 
                updateContactCurrentStatus($contact_id, $department_id);

                // Set the division status                                                                                                                       
                updateDivisionStatus($division_id, $department_id);

                // Set the company status
                updateCompanyStatus($employer_id, $department_id);
            }
        }

        // Any active status degrading down to inactive veteran
        $contact_status_sql = ("
            SELECT dcs.contact_id, DATE_ADD(dcs.activity_date, INTERVAL ". 1 * GRACE_PERIOD." MONTH) AS activity_date, dcs.status_id, ce.department_id, ce.employer_id
            FROM department_contact_status dcs
            INNER JOIN contact_employer ce
            ON dcs.contact_id = ce.contact_id
            WHERE dcs.current_status = '1' AND dcs.status_id IN ('".CONTACT_ACTIVE_NEW."','".CONTACT_ACTIVE_LAPSED."','".CONTACT_ACTIVE_VETERAN."')
            AND dcs.department_id = '".$department_id."' AND dcs.activity_date <= '".$one_grace_period_ago."'
            ");
        $contact_status_result = $GLOBALS['dbh']->Execute($contact_status_sql);

        while($contact_status_row = $contact_status_result->FetchRow())
        {
            $contact_id = $contact_status_row['contact_id'];
            $division_id = $contact_status_row['department_id'];
            $employer_id = $contact_status_row['employer_id'];
            $activity_date = $contact_status_row['activity_date'];

            $inactive_veteran_sql = ("
                INSERT INTO department_contact_status
                (status_id, contact_id, department_id, entered_on, activity_date, activity_type_id)
                VALUES
                ('".CONTACT_INACTIVE_VETERAN."', '".$contact_id."','".$department_id."', NOW(), '".$activity_date."', '".AUTOMATED."')
                ");
            $active_to_inactive_veteran++;
            $arr_active_to_inactive_veteran[] = $contact_id." ".$department_id;

            if ($DEBUG_ON) 
            {
                print $inactive_veteran_sql."<br />";
            }
            if ($ENABLE_SQL)
            {
                $GLOBALS['dbh']->Execute($inactive_veteran_sql);

                // Set the latest contact status to be the current status. 
                updateContactCurrentStatus($contact_id, $department_id);

                // Set the division status                                                                                                                       
                updateDivisionStatus($division_id, $department_id);

                // Set the company status
                updateCompanyStatus($employer_id, $department_id);
            }
        }

        // We now look for statuses that are currently active new and active lapsed. The reason is that if an employer consistently posts, all the statuses will be
        // active new or active lapsed, depending on what the status was initially. We want to upgrade those statuses to active veteran if the employer has posted
        // consistently across two terms. 

        $contact_status_sql = ("
            SELECT dcs.contact_id, dcs.activity_date, dcs.status_id, ce.department_id, ce.employer_id
            FROM department_contact_status dcs
            INNER JOIN contact_employer ce
            ON dcs.contact_id = ce.contact_id
            WHERE dcs.current_status = '1' AND dcs.status_id IN ('".CONTACT_ACTIVE_NEW."','".CONTACT_ACTIVE_LAPSED."')
            AND dcs.department_id = '".$department_id."' 
            ");
        $contact_status_result = $GLOBALS['dbh']->Execute($contact_status_sql);

        while($contact_status_row = $contact_status_result->FetchRow())
        {
            $contact_id = $contact_status_row['contact_id'];
            $division_id = $contact_status_row['department_id'];
            $employer_id = $contact_status_row['employer_id'];

            if ($contact_status_row['status_id'] == CONTACT_ACTIVE_NEW)
            {
                $scan_sql = ("
                    SELECT dcs.activity_date, dcs.status_id
                    FROM department_contact_status dcs
                    WHERE dcs.contact_id = '".$contact_id."' AND dcs.department_id = '".$department_id."' 
                    ORDER BY activity_date
                    ");
                $scan_result = $GLOBALS['dbh']->Execute($scan_sql);
                if ($DEBUG_ON)
                {
                    print $scan_sql."<br />";
                }

                $scan_array = array();
                
                // check to make sure contact only has active new and potential lead statuses, otherwise something's wrong. Someone that is active new can only
                // have active new or potential lead, anything else and it's an error. 
                while ($scan_row = $scan_result->FetchRow()) 
                {
                    if ($scan_row['status_id'] != CONTACT_ACTIVE_NEW && $scan_row['status_id'] != CONTACT_POTENTIAL_LEAD)
                    {
                        $scan_array = null;
                        print "ERROR ACTIVE NEW not set properly.<br />";
                        break;
                    }

                    $scan_array[] = $scan_row;
                }

                // There's two cases where we ignore. First case is if the contact has a potenial lead and active new status, then we know there's no need
                // to check its status because the contact has just been entered (POTENTIAL_LEAD), and then has posted a job (ACTIVE_NEW).
                for ($i = 0; $i < sizeof($scan_array); $i++)
                {
                    if ($scan_array[$i]['status_id'] == CONTACT_ACTIVE_NEW)
                    {
                        $earliest_active_new_date = $scan_array[$i]['activity_date'];
                        $add_grace_period_string = ("+".GRACE_PERIOD." Month");
                        $earliest_active_new_date_stamp = strtotime($add_grace_period_string,strtotime($earliest_active_new_date));
                        $earliest_active_new_date = date("Y-m-d",$earliest_active_new_date_stamp);

                        // see if we have any other active new statuses after 1 grace period. 
                        $count_check_sql = ("
                            SELECT department_contact_status_id
                            FROM department_contact_status
                            WHERE contact_id = '".$contact_id."' AND department_id = '".$department_id."' 
                            AND status_id = '".CONTACT_ACTIVE_NEW."' AND activity_date >= '".$earliest_active_new_date."'
                            ");
                        $count_check_result = $GLOBALS['dbh']->Execute($count_check_sql);

                        if ($count_check_result->RecordCount())
                        {
                            $active_veteran_sql = ("
                                UPDATE department_contact_status
                                SET status_id = '".CONTACT_ACTIVE_VETERAN."'
                                WHERE contact_id = '".$contact_id."' AND department_id = '".$department_id."' 
                                AND activity_date >= '".$earliest_active_new_date."'
                                AND status_id = '".CONTACT_ACTIVE_NEW."'
                                ");
                            $active_new_to_active_veteran++;
                            $arr_active_new_to_active_veteran[] = $contact_id." ".$department_id;

                            if ($DEBUG_ON)
                            {
                                print $active_veteran_sql."<br />";
                            }

                            if ($ENABLE_SQL)
                            {
                                $GLOBALS['dbh']->Execute($active_veteran_sql);

                                // Set the latest contact status to be the current status. 
                                updateContactCurrentStatus($contact_id, $department_id);

                                // Set the division status                                                                                                                       
                                updateDivisionStatus($division_id, $department_id);

                                // Set the company status
                                updateCompanyStatus($employer_id, $department_id);
                            }
                        }

                        break;
                    }
                } // end if
            } // end if

            // The other case we worry about is when someone was inactive lapsed, and begins posting consistently. Their status for all these jobs will be 
            // active lapsed; however, when they post consistently for two terms, they should be upgraded to active veteran. The following does this. 
            elseif ($contact_status_row['status_id'] == CONTACT_ACTIVE_LAPSED)
            {
                $scan_sql = ("
                    SELECT department_contact_status_id, dcs.activity_date, dcs.status_id
                    FROM department_contact_status dcs
                    WHERE dcs.contact_id = '".$contact_id."' AND dcs.department_id = '".$department_id."' 
                    ORDER BY activity_date DESC
                    ");
                $scan_result = $GLOBALS['dbh']->Execute($scan_sql);

                if ($scan_result->RecordCount())
                {
                    $i = 0;
                    $error_flag = 0;

                    while ($scan_row = $scan_result->FetchRow())
                    {
                        if ($i ==  0 && $scan_row['status_id'] != CONTACT_ACTIVE_LAPSED)
                        {
                            $error_flag = 1;
                            print "SANITY CHECK ERROR<br />";
                            break;
                        }

                        // We're going backwards in activitiy dates, and trying to find the earliest activity date of the active lapsed status. 
                        if ($scan_row['status_id'] == CONTACT_ACTIVE_LAPSED)
                        {
                            // take a date and add GRACE_PERIOD months to it. In our case, 4 months. 
                            $earliest_active_lapsed_date = $scan_row['activity_date'];
                            $add_grace_period_string = ("+".GRACE_PERIOD." Month");
                            $earliest_active_lapsed_date_stamp = strtotime($add_grace_period_string,strtotime($earliest_active_lapsed_date));
                            $earliest_active_lapsed_date = date("Y-m-d",$earliest_active_lapsed_date_stamp);
                        }
                        // When we hit a status that's not active_lapsed, we have the date of the earliest active_lapsed status change. 
                        // For example, a contact may have:
                        //                                                        <---- scanning --------
                        // [active new] [inactive veteran] [active lapsed] [active lapsed] [active lapsed]
                        //               ^ status change   ^ save activity date
                        else
                        {
                            break;
                        }

                        $i++;
                    }

                    if (!$error_flag)
                    {

                        $count_check_sql = ("
                            SELECT department_contact_status_id
                            FROM department_contact_status
                            WHERE contact_id = '".$contact_id."' AND department_id = '".$department_id."'
                            AND status_id = '".CONTACT_ACTIVE_LAPSED."' AND activity_date >= '".$earliest_active_lapsed_date."'
                            ");
                        $count_check_result = $GLOBALS['dbh']->Execute($count_check_sql);
                        if ($count_check_result->RecordCount())
                        {
                            $active_veteran_sql = ("
                                UPDATE department_contact_status
                                SET status_id = '".CONTACT_ACTIVE_VETERAN."'
                                WHERE contact_id = '".$contact_id."' AND department_id = '".$department_id."' 
                                AND activity_date >= '".$earliest_active_lapsed_date."'
                                AND status_id = '".CONTACT_ACTIVE_LAPSED."'
                                ");
                            $active_lapsed_to_active_veteran++;
                            $arr_active_lapsed_to_active_veteran[] = $contact_id." ".$department_id;

                            if ($DEBUG_ON)
                            {
                                print $active_veteran_sql."<br />";
                            }

                            if ($ENABLE_SQL)
                            {
                                $GLOBALS['dbh']->Execute($active_veteran_sql);

                                // Set the latest contact status to be the current status. 
                                updateContactCurrentStatus($contact_id, $department_id);

                                // Set the division status                                                                                                                       
                                updateDivisionStatus($division_id, $department_id);

                                // Set the company status
                                updateCompanyStatus($employer_id, $department_id);
                            }
                        }
                    }
                } // end if
            } // end elseif
        } // end while
    } // end while
}

ob_start(); 

if (sizeof($arr_inactive_veteran_to_inactive_lapsed) || sizeof($arr_active_to_inactive_veteran) || sizeof($arr_active_new_to_active_veteran) 
    || sizeof($arr_active_lapsed_to_active_veteran))
{
    $change_flag = 1;
    print "==============================\n";
    print date("F j, Y (g:i a)")."\n\n";
}

if (sizeof($arr_inactive_veteran_to_inactive_lapsed)) {
    print "INACTIVE_VETERAN -> INACTIVE_LAPSED ".$inactive_veteran_to_inactive_lapsed."\n"; 
    for ($i = 0; $i < sizeof($arr_inactive_veteran_to_inactive_lapsed); $i++)
    {
        print $arr_inactive_veteran_to_inactive_lapsed[$i]."\n";
    }
}

if (sizeof($arr_active_to_inactive_veteran)) {
    print "ACTIVE -> INACTIVE VETERAN ".$active_to_inactive_veteran."\n";
    for ($i = 0; $i < sizeof($arr_active_to_inactive_veteran); $i++)
    {
        print $arr_active_to_inactive_veteran[$i]."\n";
    }
}

if (sizeof($arr_active_new_to_active_veteran)) {
    print "ACTIVE_NEW -> ACTIVE_VETERAN ".$active_new_to_active_veteran."\n";
    for ($i = 0; $i < sizeof($arr_active_new_to_active_veteran); $i++)
    {
        print $arr_active_new_to_active_veteran[$i]."\n";
    }
}

if (sizeof($arr_active_lapsed_to_active_veteran)) {
    print "ACTIVE_LAPSED -> ACTIVE_VETERAN ".$active_lapsed_to_active_veteran."\n"; 
    for ($i = 0; $i < sizeof($arr_active_lapsed_to_active_veteran); $i++)
    {
        print $arr_active_lapsed_to_active_veteran[$i]."\n";
    }
}

if ($change_flag) {
    echo("TOTAL TIME: " . (time() - $begin_time)."\n");
    print "==============================\n";
    $output = ob_get_contents();
    $fp = fopen(PATH_LOGS . "aging_script.log", "a");
    fputs($fp, $output);
}

ob_end_clean();

?>
