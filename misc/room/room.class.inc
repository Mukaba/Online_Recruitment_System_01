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
 | Filename: room.class.inc                                                     |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/


// filename: room.class.inc

class room
{
	var $room_id;
	var $room_number;
	var $time_slots;

	function room($room_id, $room_number, $time_slots)
	{
		$this->room_id = $room_id;
		$this->room_number = $room_number;
		$this->time_slots = $time_slots;
	}


	function objToString()
	{
		$output = "Object room:(";
		$output .= "room_id: '" . $this->room_id . "'";
		$output .= ", room_number: '" . $this->room_number . "'";
		$output .= ", time_slots: {";
		$slots = count($this->time_slots);
		$slotslessone = $slots - 1;
		for ($i = 0; $i < $slots; $i++)
		{
			$output .= "'";
			$output .= $this->time_slots[$i]->objToString();
			$output .= "'";
			if ($i < $slotslessone) { $output .= ", "; }
		}
		$output .= "}";
		$output .= ")";
		return $output;
	}
}

?>