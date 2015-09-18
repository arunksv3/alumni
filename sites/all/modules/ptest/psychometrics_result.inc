<?php
/**
 * @file
 * A psychometric module that displays form to attend for test.
 */
function psychometrics_result($secid,$pid) { 
    global $user;
    $secret_key = variable_get('IIUM_Encryption_Key', NULL);
    $secid = (int)decrypt($secid, $secret_key);
    $pid = (int)decrypt($pid, $secret_key);
    
    /* query to fetch metric no from stdinfo */
    $std_query = db_select('stdinfo')
        ->fields('stdinfo', array('matric_no'))
        ->condition('uid', $user->uid, '=')
        ->execute();
    $std_info = $std_query->fetchAssoc();
    
    /* query to fetch psychometrics test based on id */
    $pt_query = db_select('psychometric_info','pinfo');
    $pt_query->join('psychometric_test','pt', 'pinfo.id = pt.pid');       
    $pt_query->fields('pinfo', array('id','uid','pt_id'));
    $pt_query->fields('pt', array('id','pid','domid','qnum','answer'));
    $pt_query->condition('pid', $pid, '=');
    $query = $pt_query->execute();
    $pt_result = $query->fetchAll(); 
    
    $pdom_query = db_select('psychometric_domains')
        ->fields('psychometric_domains')        
        ->condition('secid', $secid, '=')
        ->execute();    
    $domain_result = $pdom_query->fetchAll(); 

    $points = array();
    foreach ($pt_result as $value) {           
        array_unshift($domain_result,"");
        unset($domain_result[0]);  
        $points[$value->domid][] = calculate_points($value->qnum,$value->answer);
    }
    
    /* Table header columns */
    if($secid == "2"){
        $class = "big-five";
        $header = array(
            array('data' => t('Low Meanings')), 
            array('data' => t('Domains')), 
            array('data' => t('High Meanings'))
        );
    }else if($secid == "3"){    
        $class = "career";
        $header = array(
            array('data' => t('Domains')), 
            array('data' => t('Description'))
        );
    }else{
        $class = "career-asp";
        $header = array(
            array('data' => t('Low Meanings')), 
            array('data' => t('Moderate')), 
            array('data' => t('High Meanings'))
        );
    }

    $result_comments_arr = array();
    foreach ($domain_result as $value) { 
        if($value->secid == "2")
            $result_comments_arr[] = array($value->low_meanings, '<h1>'.$value->title.'</h1><span>'.$value->desc.'</span>', $value->high_meanings);
        else if($value->secid == "3")
            $result_comments_arr[] = array($value->title, $value->desc);
        else if($value->secid == "5"){
            $result_comments_arr[0] = array('Has low confidence in career decision making. The person has little or no idea on his capabilities. He may have a 
hard time in deciding on a specific career. The person may give little or no commitment in tasks related to his career attaintment and development. 
','Has moderate confidence in career decision making. The person has an idea on his capabilities. He may have little difficulties in deciding on a 
specific career. The person may give moderate commitment in tasks related to his career attaintment and development. 
','Has high confidence in career decision making. The person knows his capabilities and is certain to decide on specific career. The person may give
high commitment in tasks related to his career attaintment and development.');
        }
    }
    
    if(empty($domain_result)){        
        $result_comments_arr[0] = array('Possess little or no career directions and goals. The person may try to understand his/her self (strengths and 
weaknesses) and the compatibility to existing career. Probably may have issues in career decision making and future career attaintment. 
','Possess moderate career directions and goals. The person understands his self (strengths and weaknesses) and the compatibility to 
existing career. Moderate career aspiration may experince little issues in career decision making and future career attaintment. 
','Possess clear career directions and goals. The person fully understands his self (strengths and weaknesses) and the compatibility to existing career.
High career aspiration may promote better career decision making and future career attaintment.');
    }
    
    $result = '<div class="pshyc-header"> 
                <p>RESULT: IIUM STUDENT CAREER PROFILING</p>
                <h1>'.get_section_name($secid).'</h1>
                <h2>Matric Number: <span>'.$std_info['matric_no'].'</span></h2>
        </div>';
    
    $points_scored = array(); 
    $chart = array(
        '#type' => 'chart',
        '#chart_type' => 'bar',
        '#title' => t('Scoring'),
    );    
    
    $result_arr = array();
    foreach ($points as $key => $value) {
        if($secid != 4){
            if($key!=0){
                /* query to fetch psychometrics sections based on domid */
                $pq = db_select('psychometric_questions','pq');    
                $pq->fields('pq');
                $pq->condition('domid', $key, '=');
                $pq_query = $pq->execute();
                $q_count = $pq_query->rowCount();
                $points_scored[$key] = array_sum($value);     
                if($secid != 3){
                    $result_arr = array_merge($result_arr, array(get_domain_name($key) => round(($points_scored[$key]/($q_count*5))*100,'2')));           
                }else{
                    $result_arr = array_merge($result_arr, array(get_domain_name($key) => $points_scored[$key]));           
                }
            }
        }else{ 
            $ps = db_select('psychometric_sections','ps');    
            $ps->fields('ps',array('id','title'));
            $ps->condition('id', $secid, '=');
            $ps_query = $ps->execute();
            $result_arr = array_merge($result_arr, array($ps_query->fetchColumn('1') => array_sum($value)));            
        }      
    }
    
    /*echo $secid; echo '</br>';
    echo '<pre>'; print_r($result_arr); exit;*/
    
    $chart['xaxis'] = array(
        '#type' => 'chart_xaxis',
        '#labels' => array_keys($result_arr),       
        '#title' => t('Domain'),
    );
    
    if($secid == 3){
        $min = '0';
        $max = '51';
        $suffix = '';
    }else{
        $min = '0';
        $max = '101';
        $suffix = '%';
    }

    $chart['yaxis'] = array(
        '#type' => 'chart_yaxis',
        '#axis_type' => 'linear',         
        '#min' => $min,
        '#max' => $max,
    );
    
    // Test with a gap in the data.
    $chart['scoring'] = array(
        '#type' => 'chart_data',
        '#title' => t('Scoring'),        
        '#data' => array_values($result_arr),
        '#suffix' => $suffix,
        '#color' => 'red',
    );
    
    
    $thanks_msg = "";    
    $result_comments = theme('table', array('header' => $header, 'rows' => $result_comments_arr, 'empty' => t('No Records Found!')));
    
    if($user->uid != 1){    
        $thanks_msg = '<h2>Thank you for your kind commitment</h2>
                   <p>You are welcome to come for result consultation at the</p>
                   <p>Counseling Services Centre<br/> Level 2, Rectory Building<br/>
                   International Islamic University Malaysia</p>';
    }
    
    $result .= drupal_render($chart). 
            '<div class="'.$class.'">' .$result_comments. '</div> 
            <div class="thanks-msg">'.$thanks_msg.'</div>'; 
    
    return $result;
}
