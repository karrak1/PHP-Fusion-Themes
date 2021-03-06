<?php
/*-------------------------------------------------------+
| PHP-Fusion Content Management System
| Copyright (C) PHP-Fusion Inc
| https://www.php-fusion.co.uk/
+--------------------------------------------------------+
| Filename: Czechia/classes/Footer/Comments.inc
| Author: RobiNN
+--------------------------------------------------------+
| This program is released as free software under the
| Affero GPL license. You can redistribute it and/or
| modify it under the terms of this license which you
| can read by viewing the included agpl.txt or online
| at www.gnu.org/licenses/agpl.html. Removal of this
| copyright header is strictly prohibited without
| written permission from the original author(s).
+--------------------------------------------------------*/
namespace CzechiaTheme\Footer;

use CzechiaTheme\Core;

/**
 * Class Comments
 *
 * @package CzechiaTheme\Footer
 */
class Comments extends Core {
    public static function Panel() {
        $locale = fusion_get_locale();

        echo '<h3 class="title"><i class="fa fa-comments-o"></i> '.self::SetLocale('13').'</h3>';

        $comments_per_page = fusion_get_settings('comments_per_page');

        $result = dbquery("SELECT c.*, u.user_id, u.user_name, u.user_status, u.user_avatar
            FROM ".DB_COMMENTS." c
            LEFT JOIN ".DB_USERS." u ON u.user_id=c.comment_name
            WHERE c.comment_hidden=0
            ORDER BY c.comment_datestamp DESC
            LIMIT 5
        ");

        if (dbrows($result)) {
            $info = [];

            while ($data = dbarray($result)) {
                switch ($data['comment_type']) {
                    case 'A':
                        $result_a = dbquery("SELECT ar.article_subject
                            FROM ".DB_ARTICLES." AS ar
                            INNER JOIN ".DB_ARTICLE_CATS." AS ac ON ac.article_cat_id=ar.article_cat
                            WHERE ar.article_id=:id AND ar.article_draft=0
                            AND ".groupaccess('ar.article_visibility')."
                            ".(multilang_table('AR') ? " AND ar.article_language='".LANGUAGE."'" : '')."
                            ORDER BY ar.article_datestamp DESC
                        ", [':id' => $data['comment_item_id']]);

                        if (dbrows($result_a)) {
                            $article_data = dbarray($result_a);
                            $comment_start = dbcount('(comment_id)', DB_COMMENTS, "comment_item_id='".$data['comment_item_id']."' AND comment_type='A' AND comment_id<=".$data['comment_id']);
                            $comment_start = $comment_start > $comments_per_page ? '&amp;c_start_news_comments='.((floor($comment_start / $comments_per_page) * $comments_per_page) - $comments_per_page) : '';

                            $info[] = [
                                'data'  => $data,
                                'url'   => INFUSIONS.'articles/articles.php?article_id='.$data['comment_item_id'],
                                'title' => $article_data['article_subject'],
                                'c_url' => INFUSIONS.'articles/articles.php?article_id='.$data['comment_item_id'].$comment_start.'#c'.$data['comment_id']
                            ];
                        }
                        continue;
                    case 'B':
                        $result_b = dbquery("SELECT d.blog_subject
                            FROM ".DB_BLOG." AS d
                            INNER JOIN ".DB_BLOG_CATS." AS c ON c.blog_cat_id=d.blog_cat
                            WHERE d.blog_id=:id AND ".groupaccess('d.blog_visibility')."
                            ".(multilang_table('BL') ? " AND d.blog_language='".LANGUAGE."'" : '')."
                            ORDER BY d.blog_datestamp DESC
                        ", [':id' => $data['comment_item_id']]);

                        if (dbrows($result_b)) {
                            $blog_data = dbarray($result_b);
                            $comment_start = dbcount('(comment_id)', DB_COMMENTS, "comment_item_id='".$data['comment_item_id']."' AND comment_type='B' AND comment_id<=".$data['comment_id']);
                            $comment_start = $comment_start > $comments_per_page ? '&amp;c_start_news_comments='.((floor($comment_start / $comments_per_page) * $comments_per_page) - $comments_per_page) : '';

                            $info[] = [
                                'data'  => $data,
                                'url'   => INFUSIONS.'blog/blog.php?readmore='.$data['comment_item_id'],
                                'title' => $blog_data['blog_subject'],
                                'c_url' => INFUSIONS.'blog/blog.php?readmore='.$data['comment_item_id'].$comment_start.'#c'.$data['comment_id']
                            ];
                        }
                        continue;
                    case 'N':
                        $result_n = dbquery("SELECT ns.news_subject
                            FROM ".DB_NEWS." AS ns
                            LEFT JOIN ".DB_NEWS_CATS." AS nc ON nc.news_cat_id=ns.news_cat
                            WHERE ns.news_id=:id AND (ns.news_start=0 OR ns.news_start<='".TIME."')
                            AND (ns.news_end=0 OR ns.news_end>='".TIME."') AND ns.news_draft=0
                            AND ".groupaccess('ns.news_visibility')."
                            ".(multilang_table('NS') ? "AND ns.news_language='".LANGUAGE."'" : '')."
                            ORDER BY ns.news_datestamp DESC
                        ", [':id' => $data['comment_item_id']]);

                        if (dbrows($result_n)) {
                            $news_data = dbarray($result_n);
                            $comment_start = dbcount('(comment_id)', DB_COMMENTS, "comment_item_id='".$data['comment_item_id']."' AND comment_type='N' AND comment_id<=".$data['comment_id']);
                            $comment_start = $comment_start > $comments_per_page ? '&amp;c_start_news_comments='.((floor($comment_start / $comments_per_page) * $comments_per_page) - $comments_per_page) : '';

                            $info[] = [
                                'data'  => $data,
                                'url'   => INFUSIONS.'news/news.php?readmore='.$data['comment_item_id'],
                                'title' => $news_data['news_subject'],
                                'c_url' => INFUSIONS.'news/news.php?readmore='.$data['comment_item_id'].$comment_start.'#c'.$data['comment_id']
                            ];
                        }
                        continue;
                    case 'P':
                        $result_p = dbquery("SELECT p.photo_title
                            FROM ".DB_PHOTOS." AS p
                            INNER JOIN ".DB_PHOTO_ALBUMS." AS a ON p.album_id=a.album_id
                            WHERE p.photo_id=:id AND ".groupaccess('a.album_access')."
                            ".(multilang_table('PG') ? " AND a.album_language='".LANGUAGE."'" : '')."
                            ORDER BY p.photo_datestamp DESC
                        ", [':id' => $data['comment_item_id']]);

                        if (dbrows($result_p)) {
                            $photo_data = dbarray($result_p);
                            $comment_start = dbcount('(comment_id)', DB_COMMENTS, "comment_item_id='".$data['comment_item_id']."' AND comment_type='P' AND comment_id<=".$data['comment_id']);
                            $comment_start = $comment_start > $comments_per_page ? '&amp;c_start_news_comments='.((floor($comment_start / $comments_per_page) * $comments_per_page) - $comments_per_page) : '';

                            $info[] = [
                                'data'  => $data,
                                'url'   => INFUSIONS.'gallery/gallery.php?photo_id='.$data['comment_item_id'],
                                'title' => $photo_data['photo_title'],
                                'c_url' => INFUSIONS.'gallery/gallery.php?photo_id='.$data['comment_item_id'].$comment_start.'#c'.$data['comment_id']
                            ];
                        }
                        continue;
                    case 'D':
                        $result_d = dbquery("SELECT d.download_title
                            FROM ".DB_DOWNLOADS." AS d
                            INNER JOIN ".DB_DOWNLOAD_CATS." AS c ON c.download_cat_id=d.download_cat
                            WHERE d.download_id=:id AND ".groupaccess('d.download_visibility')."
                            ".(multilang_table("DL") ? " AND c.download_cat_language='".LANGUAGE."'" : '')."
                            ORDER BY d.download_datestamp DESC
                        ", [':id' => $data['comment_item_id']]);

                        if (dbrows($result_d)) {
                            $download_data = dbarray($result_d);
                            $comment_start = dbcount('(comment_id)', DB_COMMENTS, "comment_item_id='".$data['comment_item_id']."' AND comment_type='D' AND comment_id<=".$data['comment_id']);
                            $comment_start = $comment_start > $comments_per_page ? '&amp;c_start_news_comments='.((floor($comment_start / $comments_per_page) * $comments_per_page) - $comments_per_page) : '';

                            $info[] = [
                                'data'  => $data,
                                'url'   => INFUSIONS.'downloads/downloads.php?download_id='.$data['comment_item_id'],
                                'title' => $download_data['download_title'],
                                'c_url' => INFUSIONS.'downloads/downloads.php?download_id='.$data['comment_item_id'].$comment_start.'#c'.$data['comment_id']
                            ];
                        }
                        break;
                }
            }

            echo '<ul class="list-style-none break-words">';
            foreach ($info as $id => $data) {
                $link = !empty($data['data']['user_id']) ? TRUE : FALSE;

                echo '<li id="comment-'.$id.'" class="m-t-5">';
                    echo '<div class="pull-left">'.display_avatar($data['data'], '35px', '', $link, 'img-circle m-r-10 m-t-5').'</div>';
                    echo '<div class="overflow-hide">';
                        echo '<strong><a class="text-white" href="'.$data['url'].'">'.trim_text($data['title'], 40).'</a></strong>';
                        echo '<div class="clearfix"><a href="'.$data['c_url'].'">'.trim_text(strip_tags(parse_textarea($data['data']['comment_message'], FALSE, TRUE)), 35).'</a></div>';
                    echo '</div>';
                echo '</li>';
            }
            echo '</ul>';
        } else {
            echo $locale['global_026'];
        }
    }
}
