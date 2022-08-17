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
 | Filename: MamookReportPlacementSummary.class.php                             |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+
*/


if (IN_MAMOOK !== true) {
    header("HTTP/1.0 404 Not Found");
    die();
}

class MamookReportPlacementSummary extends PluginInterface {

    var $select = 'placement_summary';

    function MamookReportPlacementSummary() {
        $this->plugin_version_number = '1.0';
        $this->plugin_author = 'University of Victoria';
        $this->plugin_name = 'Placement Summary Report';
        $this->plugin_type = 'module';
    }

    function init() {
        register_hook("mamook.traffic_control.select", $this, "handleTraffic");
    }

    function handleTraffic($select) {
        global $auth;
        global $plugin_absolute_path;
        global $PHP_SELF;

        $plugin_absolute_path = $this->getPluginAbsolutePath();

        // temporary work around to local scoping of variables
        if (ini_get('register_globals')) {
            include('misc/compatibility.inc');
        }

        if ($select == $this->getSelect()) {
            if ($auth->userlevel == OFFICE || $auth->userlevel == FACULTY) {
                include($plugin_absolute_path.'placement_summary.inc');
                return true;
            }
            // access control violation
            else {
                include('misc/loguser.inc');
                return null;
            } 
        }
        else {
            return null;
        }
    }

    function getSelect() {
        return $this->select;
    }
}

?>
