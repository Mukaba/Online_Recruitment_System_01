<?php
session_start();
ini_set('display_errors', 1);
Class Action {
	private $db;

	public function __construct() {
		ob_start();
   	include 'include/database/pdo_connect.php';
    
    $this->db = $conn;
	}
	function __destruct() {
	    $this->db->close();
	    ob_end_flush();
	}

	function login(){
		extract($_POST);
		$qry = $this->db->query("SELECT * FROM users where username = '".$username."' and password = '".md5($password)."' ");
		if($qry->num_rows > 0){
			foreach ($qry->fetch_array() as $key => $value) {
				if($key != 'passwors' && !is_numeric($key))
					$_SESSION['login_'.$key] = $value;
			}
				return 1;
		}else{
			return 3;
		}
	}
	function logout(){
		session_destroy();
		foreach ($_SESSION as $key => $value) {
			unset($_SESSION[$key]);
		}
		header("location:login2.php");
	}

	function save_user(){
		extract($_POST);
		$data = " name = '$name' ";
		$data .= ", username = '$username' ";
		if(!empty($password))
		$data .= ", password = '".md5($password)."' ";
		if(isset($type))
		$data .= ", type = '$type' ";
		$chk = $this->db->query("Select * from users where username = '$username' and id !='$id' ")->num_rows;
		if($chk > 0){
			return 2;
			exit;
		}
		if(empty($id)){
			$save = $this->db->query("INSERT INTO users set ".$data);
		}else{
			$save = $this->db->query("UPDATE users set ".$data." where id = ".$id);
		}
		if($save){
			return 1;
		}
	}
	function delete_user(){
		extract($_POST);
		$delete = $this->db->query("DELETE FROM users where id = ".$id);
		if($delete)
			return 1;
	}
	function signup(){
		extract($_POST);
		$data = " name = '$name' ";
		$data .= ", contact = '$contact' ";
		$data .= ", address = '$address' ";
		$data .= ", username = '$email' ";
		$data .= ", password = '".md5($password)."' ";
		$data .= ", type = 3";
		$chk = $this->db->query("SELECT * FROM users where username = '$email' ")->num_rows;
		if($chk > 0){
			return 2;
			exit;
		}
			$save = $this->db->query("INSERT INTO users set ".$data);
		if($save){
			$qry = $this->db->query("SELECT * FROM users where username = '".$email."' and password = '".md5($password)."' ");
			if($qry->num_rows > 0){
				foreach ($qry->fetch_array() as $key => $value) {
					if($key != 'passwors' && !is_numeric($key))
						$_SESSION['login_'.$key] = $value;
				}
			}
			return 1;
		}
	}

	function save_settings(){
		extract($_POST);
		$data = " name = '".str_replace("'","&#x2019;",$name)."' ";
		$data .= ", email = '$email' ";
		$data .= ", contact = '$contact' ";
		$data .= ", about_content = '".htmlentities(str_replace("'","&#x2019;",$about))."' ";
		if($_FILES['img']['tmp_name'] != ''){
						$fname = strtotime(date('y-m-d H:i')).'_'.$_FILES['img']['name'];
						$move = move_uploaded_file($_FILES['img']['tmp_name'],'assets/img/'. $fname);
					$data .= ", cover_img = '$fname' ";

		}
		
		// echo "INSERT INTO system_settings set ".$data;
		$chk = $this->db->query("SELECT * FROM system_settings");
		if($chk->num_rows > 0){
			$save = $this->db->query("UPDATE system_settings set ".$data);
		}else{
			$save = $this->db->query("INSERT INTO system_settings set ".$data);
		}
		if($save){
		$query = $this->db->query("SELECT * FROM system_settings limit 1")->fetch_array();
		foreach ($query as $key => $value) {
			if(!is_numeric($key))
				$_SESSION['setting_'.$key] = $value;
		}

			return 1;
				}
	}

	
	function save_quiz(){
		extract($_POST);
		$data = " title = '$title' ";
		$data .= ", user_id = '$user_id' ";
		$cwhere ='';
		if(!empty($id)){
			$cwhere = " and id != $id ";
		}
		$chk =  $this->db->query("SELECT * FROM quiz_list where  title = '$title' and user_id = $user_id ".$cwhere)->num_rows;
		if($chk > 0){
			return 2;
			exit;
		}
		if(empty($id)){
			$save = $this->db->query("INSERT INTO quiz_list set ".$data);
		}else{
			$save = $this->db->query("UPDATE quiz_list set ".$data." where id=".$id);
		}
		if($save)
			return 1;
	}
	function delete_quiz(){
		extract($_POST);
		$delete = $this->db->query("DELETE FROM quiz_list where id = ".$id);
		if($delete)
			return 1;
	}
	
	function save_question(){
		extract($_POST);
		if(empty($id)){
		$last_order = $this->db->query("SELECT * FROM questions where qid = $qid order by order_by desc limit 1")->fetch_array()['order_by'];
		$order_by = $last_order > 0 ? $last_order + 1 : 0;
		$data = 'question = "'.$question.'" ';
		$data .= ', order_by = "'.$order_by.'" ';
		$data .= ', qid = "'.$qid.'" ';
		$data .= ', points = "'.$points.'" ';

			$insert_question = $this->db->query("INSERT INTO questions set ".$data);
			if($insert_question){
				$question_id = $this->db->insert_id;
				$save = array();
				for($i = 0 ; $i < count($question_opt);$i++){
					$is_right = isset($is_right[$i]) ? $is_right[$i] : 0;
					$save[] = $this->db->query("INSERT INTO question_opt set question_id = $question_id, option_txt = '".$question_opt[$i]."',`is_right` = $is_right ");
				}
				if(count($save) == 4){
					return 1;
				}else{
					$delete = $this->db->query("DELETE FROM questions where id =".$question_id);
					$delete2 = $this->db->query("DELETE FROM question_opt where question_id =".$question_id);
					return 2;
					
				}
				
			}
		}else{
			$data = 'question = "'.$question.'" ';
			$data .= ', qid = "'.$qid.'" ';
			$data .= ', points = "'.$points.'" ';
			$update = $this->db->query("UPDATE questions set ".$data." where id = ".$id);
			if($update){
					$delete= $this->db->query("DELETE FROM question_opt where question_id =".$id);
					$save = array();
					for($i = 0 ; $i < count($question_opt);$i++){
						$answer = isset($is_right[$i]) ? 1 : 0;
						$save[] = $this->db->query("INSERT INTO question_opt set question_id = $id, option_txt = '".$question_opt[$i]."',`is_right` = $answer ");
					}
					if(count($save) == 4){
						return 1;
					}else{
						$delete = $this->db->query("DELETE FROM questions where id =".$id);
						$delete2 = $this->db->query("DELETE FROM question_opt where question_id =".$id);
						return 2;
						
					}

				}
		}
		if(isset($save))
			return 1;
	}
	function delete_question(){
		extract($_POST);
		$delete = $this->db->query("DELETE FROM questions where id =".$id);
		 $this->db->query("DELETE FROM question_opt where question_id =".$id);
		if($delete)
			return 1;
	}
	function check_quiz_exist(){
		extract($_POST);
		$chk = $this->db->query("SELECT * from quiz_list where md5(id) = '$code' ")->num_rows;
		if($chk > 0){
			return 1;
		}else{
			return false;
		}
	}
	function save_participant(){
		extract($_POST);
		$id = $this->db->query("SELECT * FROM quiz_list where md5(id) = '$code' ")->fetch_array()['id'];
		$user_id = $_SESSION['login_id'];
		
		$chck = $this->db->query("SELECT * FROM participants where user_id = '$user_id' and quiz_id = $id ")->num_rows;

		if($chck > 0 ){
			 	$this->db->query("UPDATE participants set status = 1 where user_id = '$user_id' and quiz_id = $id ");
				return 1;
		}else{
			 $this->db->query("INSERT INTO participants set user_id = '$user_id' , quiz_id = $id ");
				return 1;
		}
	}
	function disconnect_all(){
		extract($_POST);
		$save = $this->db->query("UPDATE participants set status = 1 where quiz_id = ".$id);
		if($save){
			return 1;
		}
	}
	function start_quiz(){
		extract($_POST);

		$save = $this->db->query("UPDATE quiz_list set status = 2 where id = ".$id);
		$data = array();
		if($save){
			$data['status'] = 1;
			$next=json_decode($this->get_qid($id));
			$data['question_id'] = $next->question_id;
			$data['is_last'] = $next->pending > 0 ? 0 : 1;
			return json_encode($data);
		}
	}
	function start_quiz_next(){
		extract($_POST);

		$save = $this->db->query("UPDATE questions set status = 1 where id = ".$id);
		$data = array();
		if($save){
			$data['status'] = 1;
			$next=json_decode($this->get_qid($qid));
			$data['question_id'] = $next->question_id;
			$data['is_last'] = $next->pending > 0 ? 0 : 1;
			return json_encode($data);
		}
	}
	function finish_quiz(){
		extract($_POST);

		$save = $this->db->query("UPDATE quiz_list set status = 3 where id = ".$qid);
		$data = array();
		if($save){
			$data['status'] = 1;
			return json_encode($data);
		}
	}
	function get_qid($qid){
		$question = $this->db->query("SELECT * FROM questions where qid = $qid and status = 0 order by id asc limit 1")->fetch_array()['id'];
		$pending = $this->db->query("SELECT * FROM questions where qid = $qid and status = 0 and id != '$question' ")->num_rows;

		return json_encode(array("question_id"=>$question,"pending"=>$pending));
	}
	function save_answer(){
		extract($_POST);
		$opt = $this->db->query("SELECT * FROM question_opt where id = $option_id ")->fetch_array();
		$is_right = $opt['is_right'];
		$question_id = $opt['question_id'];
		$user_id = $_SESSION['login_id'];
		$data = " user_id = $user_id ";
		$data .= " ,quiz_id = $quiz_id ";
		$data .= " ,question_id = $question_id ";
		$data .= " ,option_id = $option_id ";
		$data .= " ,is_right = $is_right ";
		$chk = $this->db->query("SELECT * FROM answers where user_id = $user_id and question_id = $question_id ");
		if($chk->num_rows > 0){
			$id = $chk->fetch_array()['id'];
			$save = $this->db->query("UPDATE answers set $data where id= $id ");
		}else{
			$save = $this->db->query("INSERT INTO answers set $data  ");

		}
		if($save)
		return 1;
	}
	function load_answered(){
		extract($_POST);
		$answers = $this->db->query("SELECT * FROM answers where quiz_id = $id ");
		$list = array();
		while($row=$answers->fetch_array()){
			if(!isset($list[$row['option_id']]))
				$list[$row['option_id']] = 0;
			$list[$row['option_id']] += 1;
		}
		return json_encode($list);
	}
	function load_my_answer(){
		extract($_POST);
		$answers = $this->db->query("SELECT * FROM answers where quiz_id = $id  and user_id =".$_SESSION['login_id']);
		$list = array();
		while($row=$answers->fetch_array()){
			if(!isset($list[$row['option_id']]))
				$list[$row['option_id']] = 0;
			$list[$row['option_id']] += 1;
		}
		return json_encode($list);
	}
}