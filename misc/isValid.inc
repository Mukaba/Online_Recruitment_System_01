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
 | Filename: isValid.inc                                                        |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/


// filname: isValid.inc
// to view file properly set tab width to 3

// available functions:
// boolean isValidBuilding(int $building_id)
// boolean isValidRoom(int $room_id)
// boolean isValidInterviewRoom(int $room_id)
// boolean isValidWaitRoom(int $room_id)




// function isValidBuilding(int $building_id)
// returns:	true  - if $building_id is a valid building_id
//				false - if $building_id not a valid building_id
function isValidBuilding($building_id)
{
	$result = $GLOBALS['dbh']->Execute("
		SELECT	building_id
		FROM	building
		WHERE	building_id='" . addslashes($building_id) . "'");

	if (!$result) { return false; }
	$is_valid_building = ($result->RecordCount() == 1);
	$result->Close();

	return $is_valid_building;
}





// function isValidRoom(int $room_id)
// returns:	true  - if $room_id is a valid room_id
//				false - if $room_id not a valid room_id
function isValidRoom($room_id, $room_type="")
{
	$result = $GLOBALS['dbh']->Execute("
		SELECT	room_id
		FROM	room
		WHERE	room_id='" . addslashes($room_id) . "'
		" . (($room_type == "interview") ? " AND NOT ISNULL(wait_room_id)" : "") . "
		" . (($room_type == "wait") ? " AND ISNULL(wait_room_id)" : "") . "
		");

	if (!$result) { return false; }
	$is_valid_room = ($result->RecordCount() == 1);
	$result->Close();

	return $is_valid_room;
}


// function isValidInterviewRoom(int $room_id)
// returns:	true  - if $room_id is a valid interview room_id
//				false - if $room_id not a valid interview room_id
function isValidInterviewRoom($room_id)
{
	return isValidRoom($room_id, "interview");
}


// function isValidWaitRoom(int $room_id)
// returns:	true  - if $room_id is a valid wait room_id
//				false - if $room_id not a valid wait room_id
function isValidWaitRoom($room_id)
{
	return isValidRoom($room_id, "wait");
}


?>
