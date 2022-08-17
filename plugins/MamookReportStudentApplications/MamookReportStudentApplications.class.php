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
 | Filename: MamookReportStudentApplications.class.php                          |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+
*/


if (IN_MAMOOK !== true) {
    header("HTTP/1.0 404 Not Found");
    die();
}

class MamookReportStudentApplications extends PluginInterface {

    function MamookReportStudentApplications() {
        $this->plugin_version_number = '1.0';
        $this->plugin_author = 'University of Victoria';
        $this->plugin_name = 'Student Applications Report';
        $this->plugin_type = 'student_search_results_plugin';
    }

    function init() {
        register_hook("mamook.traffic_control.select", $this, "handleTraffic");
        register_hook("mamook.student_search_results.button", $this, "getButtonAndForm");
    }

    function handleTraffic($select) {
        global $auth;
        global $PHP_SELF;

        $plugin_absolute_path = $this->getPluginAbsolutePath();

        // temporary work around to local scoping of variables
        if (ini_get('register_globals')) {
            include('misc/compatibility.inc');
        }

        switch ($select) {
            //:TODO: Unify both cases into one 
            case "student_applications_report_form":
                if ($auth->userlevel == OFFICE || $auth->userlevel == TRAFFICDIRECTOR || $auth->userlevel == FACULTY) {
                    include($plugin_absolute_path.'main.inc');
                    return true;
                }
                else {
                    include('misc/loguser.inc');
                    return null;
                }

            case "student_applications_report":
                if ($auth->userlevel == OFFICE || $auth->userlevel == TRAFFICDIRECTOR || $auth->userlevel == FACULTY) {
                    include($plugin_absolute_path.'student_applications_report.inc');
                    return true;
                }
                else {
                    include('misc/loguser.inc');
                    return null;
                }
            default:
                return null;
        }
    }

    // $arr_params['record_id_list'] is the $record_id_list
    // $arr_params['PHP_SELF'] is $PHP_SELF
    function getButtonAndForm($arr_params) {
        $arr_return['button'] = "<input type='button' value='Student Applications Report' onclick='javascript:document.student_apps_report_form.submit()' /> ";

        $string = ("<form name='student_apps_report_form' method='post' action='".$arr_params['PHP_SELF']."&amp;select=student_applications_report_form'>");         
        $string .= ("<input type='hidden' name='record_id_list' value='".$arr_params['record_id_list']."' />");                                                         
        $string .= ("<input type='hidden' name='origin' value='student_search' />");                                                                    
        $string .= ("</form>\n\n");

        $arr_return['form'] = $string;

        return $arr_return;
    }
}

?>
