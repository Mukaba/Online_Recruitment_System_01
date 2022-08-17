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
 | Filename: constants.inc                                                      |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/
define("IN_MAMOOK", true);

# Userlevels

define("STUDENT", 1);
define("USERLEVEL_PUBLIC", 2);
define("LIMBO", 3);
define("OFFICE", 4);
define("HREMPLOYER", 5);
define("EMPLOYER", 6);
define("ADMINSTUDENT", 7);
define("FACULTY", 8);
define("ADMINISTRATOR", 9);
define("MULTIPLE", 10);
define("ADMINEMPLOYER", 11);

define ("NOBODY", 80);
define("TRAFFICDIRECTOR", 88);
define("NEW_EMPLOYER", 89);
define("FORGOTTEN_PASSWORD", 90);
define ("ALL", 99);

define("TRAFFIC_DIRECTOR_ID", 5506);


define("TRAFFIC_DIRECTOR_DEPARTMENT", 18);
define("DIRECTORS_OFFICE_DEPARTMENT", 15);

define("DEFAULT_WORK_TERM_LENGTH",4);
define("DEFAULT_WORK_TERM_HOURS_PER_WEEK",35);

# admin = 0 ; student = 1 ; public = 2; limbo = 3

/*All array menu constants that used to be defined here are now stored in the database*/

if (ROOM_BOOKING_ENABLED)
{
	//then need to add the room booking menu to the menu_item or menu_sub_item tables in db
}

define("STUDENT_PHOTO_PLACEHOLDER", "[*[*[STUDENTIMAGE]*]*]");

// see misc/wwwclient.inc
define("UNKNOWN", -1);
define("NETSCAPE", 0); // browser often identify themselves as Netscape browsers (this check v.4 | 5 only)
define("IE", 1);
define("GECKO", 2); // i.e. Mozilla, Netscape 6, possibly others
define("LYNX", 3);
define("SAFARI", 4);
define("FIREFOX", 5);
         
define("WIN", 0);
define("MAC", 1);
define("UNIX", 2); // not yet detected
define("LINUX", 3);

define("LOGIN_LOWER_CASE", 1); //all loginIDs to be converted to lower case

// ternary returns
// beware...
//  (T_TRUE) is true, (T_FALSE) is false, but (T_ERROR) is true!
define("T_TRUE", 1);
define("T_FALSE", 0);
define("T_ERROR", -1);


//interview types
define("PHONE", "Phone");
define("INPERSON", "In-person");
define("VIDEOCONFERENCE", "Video Conference");
define("SWITCHING", "Switching");
define("SIMULTANEOUS", "Simultaneous");
define("SEQUENTIAL", "Sequential");
define("ONCAMPUS", "On Campus");
define("OFFCAMPUS", "Off Campus");
define("ONOFFCAMPUS", "On/Off Campus");
define("STANDARD", "Standard");
define("SEQUENTIAL", "Sequential");

//Constants use to store links to department and company fields.

define("USE_COMPANY", "Use Company");
define("USE_DEPARTMENT", "Use Department");
define("USE_OWN", "Use Own");


//Constants used for job statuses

define("NOT_CHECKED", "1");
define("CHECKED", "2");
define("POSTED", "3");
define("CLOSED", "4");
define("CANCELLED", "5");
define("REJECTED", "6");
define("INT_ONLY", "7");
define("HOLDING", "8");

//Constants used for admin statuses
define("NOT_CHECKED_EMPLOYER", "1");
define("NOT_CHECKED_STAFF", "2");
define("REJECTED_JOB", "3");
define("REJECTED_EMPLOYER", "4");
define("REJECTED_NOT_REAL_EMPLOYER", "5");
define("REJECTED_OTHER", "6");
define("CHECKED_CODED", "7");
define("CHECKED_WAITING_DATE", "8");
define("CHECKED_WAITING_CONFIRM", "9");
define("CANCELLED_DELETED", "10");
define("POSTED_VIEWABLE", "11");
define("ADMIN_STATUS_CLOSED", "12");
define("NOT_INT_SUIT_APPS", "13");
define("NOT_INT_FUNDING", "14");
define("NOT_INT_OTHER", "15");
define("INTERVIEWING", "16");
define("WAIT_RANK", "17");
define("RANK_RECV", "18");
define("FILLED_UVIC", "19");
define("FILLED_ELSEWHERE", "20");
define("UNFILLED_SUIT_APPS", "21");
define("UNFILLED_NOT_HIRING", "22");
define("HIRING_HOLD", "23");
define("HOLDING_ADMIN", "25");

//Constants for student statuses
define("STUDENT_ACCEPTING_APP",1);
define("STUDENT_NOT_ACCEPTING_APP",2);
define("STUDENT_SHORTLIST",3);
define("STUDENT_WAIT_RANKING",4);
define("STUDENT_HIRED_UVIC",5);
define("STUDENT_HIRED_ELSEWHERE",6);
define("STUDENT_NOT_HIRING",7);

//Constants for application statuses

define("APP_APPLIED_ONLINE",0);
define("APP_ACTIVATED_ONLINE",1);
define("APP_ADMIN_DELETED",2);
define("APP_STUDENT_DELETED",3);
define("APP_UNAVAILABLE",4);
define("APP_PAPER_APPLIED",5);
define("APP_PAPER_SENT",6);
define("APP_SPECIAL",7);

//Constants for job's apply method
define("JOB_APP_METHOD_PAPER", "paper");
define("JOB_APP_METHOD_ONLINE", "online");
define("JOB_APP_METHOD_SPECIAL", "special");

//Constants used to identify special companies

define("WT_CHALLENGE","2142");
define("WT_TRANSFER","1864");

//Constants used to identify special flags

define("CURRENT_FLAG","7");
define("ELIGIBLE_FLAG","4");
define("PLACED_FLAG","5");
define("APPLIED_FLAG","55");

//Constants for flag types

define("GENERAL_FLAGS",1);
define("PROFILE_FLAGS",2);
define("TERM_FLAGS",4);

// Constants used for job hiring status

define("PENDING", "1");
define("UVIC", "2");
define("ELSEWHERE", "3");
define("NOT_HIRING", "4");

// Constant used with disicplines

define("ALL_DISCIPLINES", "1");
define("MBA_FULL", "84");
define("MBA_PART", "85");
define("PE_LEISURE_SERVICE", "92");
define("KINESIOLOGY", "91");
define("SEOS","");
define("COACHING_STUDIES","89");
define("EDUCATION","90");

// Constants used university/organization wide
define("FALL_TERM","1");
define("SPRING_TERM","2");
define("SUMMER_TERM","3");

define("SEMESTER_TABLE_YEARS", "6");
define("TERMS_IN_YEAR", "3");

// Constants used for placement history site visits by non-staff
// Values refer to contact_id of associated internal contacts
define("SITE_VISIT_FACULTY","29830");
define("SITE_VISIT_OTHER","29831");

// constants used in applications to know what what to display (all, just letters, just resumes)
define ("WHOLE_APP", 1);
define ("JUST_LETTER", 2);
define ("JUST_RESUME", 3);

// coverletter types
define ("SPECIFIC_COVERLETTER", 1);
define ("TEMPLATE_COVERLETTER", 2);
define ("GENERIC_COVERLETTER", 3);

// resume types
define ("SPECIFIC_RESUME", 1);
define ("TEMPLATE_RESUME", 2);
define ("GENERIC_RESUME", 3);

// region constants
define ("VICTORIA",1);
define ("COWICHAN",2);
define ("NANAIMO",3);
define ("OTHER_ISLAND",4);
define ("LOWER_MAINLAND",5);
define ("GREATER_VANCOUVER",8);
define ("THOMPSON",6);
define ("NORTHERN_BC",7);
define ("OTHER_BC",30);
define ("KOOTENAY",35);
define ("CARIBOO",36);

// province constants
define ("BC",1);
define ("AB",2);
define ("SK",12);
define ("MB",3);
define ("ON",9);
define ("PQ",11);
define ("NB",4);
define ("NS",7);
define ("PEI",10);
define ("NF",5);
define ("YK",13);
define ("NWT",6);
define ("NU",8);

// Country constants
define ("CANADA",1);
define ("USA",2);

// Default work term hours per week for various employer types
// PB = Private Business, FG = Federal Government, PG = Provincial Government
define ("PB_HOURS_PER_WEEK",35.00);
define ("FG_HOURS_PER_WEEK",37.50);
define ("PG_HOURS_PER_WEEK",40.00);

// Employer Types
define ("FG",1);
define ("PG",2);
define ("MG",3);
define ("PB",4);
define ("FA",5);
define ("PA",6);
define ("NPO",7);

// Semester Term Types. Refer to the semesters_description_type table
define ("WORK_TERM_SEMESTER",1);
define ("ACADEMIC_SEMESTER",2);
define ("MISC_SEMESTER",3);
define ("OFF_SEMESTER",4);

// Contact type constants
define("ACTIVE", 1);
define("POTENTIAL_LEAD", 2);
define("POTENTIAL_SOLICITED", 3);
define("OUT_OF_CONTACT", 4);
define("NON_HIRING", 5);
define("DO_NOT_CONTACT", 6);
define("DELETED", 7);

// Special status flags from employer_info_status_flags
define("CONTACT_DO_NOT_CONTACT_FLAG",1);
define("CONTACT_DELETED_FLAG",2);

// Symbol used to denote an inactive staff member. When their netlink_id = *
// the staff member is inactive. 
define("CONTACT_INTERNAL_INACTIVE","*");

// Status constants from employer_info_statuses
define("CONTACT_ACTIVE_VETERAN", 1);
define("CONTACT_ACTIVE_LAPSED", 2);
define("CONTACT_ACTIVE_NEW", 3);
define("CONTACT_INACTIVE_VETERAN", 4);
define("CONTACT_INACTIVE_LAPSED", 5);
define("CONTACT_POTENTIAL_LEAD", 6);

// Activity types from employer_info_activity_type
define("ACTIVITY",1);
define("AUTOMATED",2);

define("GRACE_PERIOD",4);

// Job admin statuses
define("NO_INTERVIEW_NO_SUITABLE_APPS", 13);
define("NO_INTERVIEW_NO_FUNDING", 14);
define("NO_INTERVIEW_OTHER", 15);


// Save search constant
define("STUDENT_ADV_SEARCH",1);
define("HISTORY_ADV_SEARCH",2);
define("COMPANY_DIVISION_ADV_SEARCH",3);
define("CONTACT_ADV_SEARCH",4);

// contact actions
define("CONDUCTED_WORKSITE_VISIT", "8");
define("INT_CONDUCTED","10");
define("JOB_DESC_RECEIVED","11");
define("FINAL_PLACEMENT","9");
define("GENERIC_NOTE", "23");

// contact action methods
define("IN_PERSON", "1");
define("PHONE_METHOD", "2");
define("EMAIL_METHOD", "3");
define("FAX", "4");
define("MAIL", "5");
define("OTHER_METHOD", "7");
define("LEFT_MESSAGE", "6");

//site visit type ids
define("ON_SITE_VISIT", "1");
define("PHONE_ID", "2");
define("EMAIL", "3");
define("ON_CAMPUS", "4");
define("OTHER", "5");

//interview method ids
define("INT_IN_PERSON","1");
define("INT_PHONE","2");
define("VIDEO_CONF","3");
define("INT_OTHER","4");

// Salary units from salary_period table
define("SALARY_PERIOD_HOUR",1);
define("SALARY_PERIOD_WEEK",2);
define("SALARY_PERIOD_2WEEK",3);
define("SALARY_PERIOD_MONTH",4);
define("SALARY_PERIOD_YEAR",5);

define("HISTORY_TERM_ROLLOVER_GRACE_PERIOD", 14);

### You should not need to modify anything below this line ###

// This is for the object packing functions. Without these two defines
// and PACK_SECRET_KEY (in secured_constants.inc), no encryption will take place.
define("PACK_CRYPT_TYPE", MCRYPT_BLOWFISH);
define("PACK_CRYPT_MODE", MCRYPT_MODE_ECB);

// Don't set this to 0, the packed data actually gets bigger, and the functions
// get slower due to more encryption happening. There doesn't actually seem to be
// a difference from 3 upwards, except speed loss, so 2 or 3 seems to be the best place.
define("PACK_COMPRESSION_LEVEL", 2 );

?>
