/**
 * 用于匹配非终结符/终结符
 * symTab装载:
 *  //block (duplicated)
 *  for
 *  while
 *  dowhile
 *  if
 *  else
 *  switch
 *  idtail
 **/
#include "parser.h"
#include "parserSign.h"
using namespace std;

//将本层的节点装载到上层，搜索下层的子节点并在下层装配至此层节点中
matchInfo Parser::program(Nonterminal* father) {
    // //如果词记号到头，则直接返回
    // if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(PROGRAM);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //terminal
    auto segmentRes = segment(son);
    if(!segmentRes.status) {
        tokenIterator = last;
        if(scan()->tag == END) {
            // cout<<"over"<<endl;
        }
        else {
            // cout<<"false "<<segmentRes.info<<endl;
            return {false, "segment > " + segmentRes.info};
        }
    }
    else program(son);

    father->setChild(son);
    return {true, "program"};
}
matchInfo Parser::segment(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(SEGMENT);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //terminal
    if(scan()->tag == KW_EXTERN) {
        Terminal* tagSon = new Terminal(scan()->tag);
        son->setChild(tagSon);
        move();
    } 
    //nonterminal
    auto typeRes = type(son);
    if(typeRes.status == false) { 
        tokenIterator = last; 
        return {false, "type > " + typeRes.info}; 
    }
    //nonterminal
    auto defRes = def(son);
    if(defRes.status == false){ 
        tokenIterator = last; 
        return {false, "def > " + defRes.info};
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::type(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(TYPE);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //terminal
    if(scan()->tag == KW_INT || scan()->tag == KW_VOID || scan()->tag == KW_CHAR) {
        Terminal* tagSon = new Terminal(scan()->tag);
        son->setChild(tagSon);
        move();
    } else {
        tokenIterator = last;
        return {false, "(KW_INT | KW_VOID | KW_CHAR)"};
    }
    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::def(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(DEF);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //terminal
    if(scan()->tag == ID) {
        Terminal* tagSon = new Terminal(scan()->tag);
        son->setChild(tagSon);
        move();
        //nonterminal
        auto idtailRes = idtail(son);
        if(idtailRes.status == false) {
            tokenIterator = last; 
            return {false, "idtail > " + idtailRes.info};
        }
    //terminal
    } else if(scan()->tag == MUL) {
        Terminal* tagSon = new Terminal(scan()->tag);
        son->setChild(tagSon);
        move();
        //terminal
        if(scan()->tag == ID) {
            Terminal* tagSon = new Terminal(scan()->tag);
            son->setChild(tagSon);
            move();
            //nonterminal
            auto initRes = init(son);
            if(initRes.status == false) {
                tokenIterator = last; 
                return {false, "init > " + initRes.info};
            }
            //nonterminal
            auto deflistRes = deflist(son);
            if(deflistRes.status == false) {
                tokenIterator = last; 
                return {false, "deflist > " + deflistRes.info};
            }
        } else {
            tokenIterator = last;
            return {false, "ID"};
        }
    } else {
        tokenIterator = last;
        return {false, "(ID | MUL)"};
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
//fixed
matchInfo Parser::idtail(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(IDTAIL);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    bool secondFlag = false;
    auto varrdefRes = varrdef(son);
    //nonterminal
    if(!varrdefRes.status) {
        //back
        tokenIterator = last;
        secondFlag = true;
    //nonterminal
    } else {
        auto deflistRes = deflist(son);
        if(deflistRes.status == false) {
            tokenIterator = last;
            secondFlag = true;
        }
    }
    if(secondFlag) {
       //terminal
        if(scan()->tag == LPAREN) {
            symTab.enter();
            Terminal* tagSon = new Terminal(scan()->tag);
            son->setChild(tagSon);
            move();

            //nonterminal
            auto paraRes = para(son);
            if(paraRes.status) {
                //terminal
                if(scan()->tag == RPAREN) {
                    Terminal* tagSon = new Terminal(scan()->tag);
                    son->setChild(tagSon);
                    move();

                    //nonterminal
                    auto funtailRes = funtail(son);
                    if(!funtailRes.status) {
                        tokenIterator = last;
                        return {false, "funtail > " + funtailRes.info};
                    }
                } else {
                    tokenIterator = last;
                    return {false, "rparen"};
                }
            } else {
                tokenIterator = last;
                return {false, "para > " + paraRes.info};
            }
            symTab.leave();
        } else {
            tokenIterator = last;
            return {false, "(varrdef | LRAREN) > " + varrdefRes.info};
        }
    }
    
    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::init(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(INIT);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //terminal
    if(scan()->tag == ASSIGN) {
        Terminal* tagSon = new Terminal(scan()->tag);
        son->setChild(tagSon);
        move();
        //nonterminal
        auto exprRes = expr(son);
        if(!exprRes.status) {
            //terminal
            tokenIterator = last;
            
        }
    } else {
        //terminal
        tokenIterator = last;
        
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::deflist(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(DEFLIST);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //terminal
    if(scan()->tag == COMMA) {
        Terminal* tagSon = new Terminal(scan()->tag);
        son->setChild(tagSon);
        move();
        //nonterminal
        auto defdataRes = defdata(son);
        if(!defdataRes.status) {
            tokenIterator = last;
            return {false, "defdata > " + defdataRes.info};
        } else {
            //nonterminal
            auto deflistRes = deflist(son);
            if(!deflistRes.status) {
                tokenIterator = last;
                return {false, "deflist > " + deflistRes.info};
            }
        }
    } else {
        //back
        tokenIterator = last;

        if(scan()->tag == SEMICON) {
            Terminal* tagSon = new Terminal(scan()->tag);
            son->setChild(tagSon);
            move();
        } else {
            return {false, "(SEMICON | COMMA)"};
        }
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::varrdef(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(VARRDEF);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //terminal
    if(scan()->tag == LBRACK) {
        Terminal* tagSon = new Terminal(scan()->tag);
        son->setChild(tagSon);
        move();
        //terminal
        if(scan()->tag == NUM) {
            Terminal* tagSon = new Terminal(scan()->tag);
            son->setChild(tagSon);
            move();
            //terminal
            if(scan()->tag == RBRACK) {
                Terminal* tagSon = new Terminal(scan()->tag);
                son->setChild(tagSon);
                move();  
            } else {
                tokenIterator = last;
                return {false, "RBRACK"};
            }
        } else {
            tokenIterator = last;
            return {false, "NUM"};
        }
    } else {
        //back
        tokenIterator = last;
        //nonterminal
        auto initRes = init(son);
        if(!initRes.status) {
            tokenIterator = last;
            return {false, "(LBRACK | init) > " + initRes.info};
        }
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::para(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(PARA);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //nonterminal
    auto typeRes = type(son);
    if(typeRes.status) {
        //nonterminal
        auto paradataRes = paradata(son);
        if(paradataRes.status) {
            //nonterminal
            auto paralistRes = paralist(son);
            if(!paralistRes.status) {
                //terminal
                tokenIterator = last;
                
            }
        } else {
            //terminal
            tokenIterator = last;
            
        }
    } else {
        //terminal
        tokenIterator = last;
        
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::funtail(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(FUNTAIL);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //terminal
    if(scan()->tag == SEMICON) {
        Terminal* tagSon = new Terminal(scan()->tag);
        son->setChild(tagSon);
        move();
    } else {
        //nonterminal
        auto blockRes = block(son);
        if(!blockRes.status) {
            tokenIterator = last;
            return {false, "(SEMICON | BLOCK) > "};
        }
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::expr(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(EXPR);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //nonterminal
    auto assexprRes = assexpr(son);
    if(!assexprRes.status) {
        tokenIterator = last;
        return {false, "assexpr > "};
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::altexpr(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(ALTEXPR);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //nonterminal
    auto exprRes = expr(son);
    if(!exprRes.status) {
        tokenIterator = last;
        
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::defdata(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(DEFDATA);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //terminal
    if(scan()->tag == ID) {
        Terminal* tagSon = new Terminal(scan()->tag);
        son->setChild(tagSon);
        move();
        //nonterminal
        auto varrdefRes = varrdef(son);
        if(!varrdefRes .status) {
            tokenIterator = last;
            return {false, "varrdef > " + varrdefRes.info};
        }
    //terminal
    } else if(scan()->tag == MUL) {
        Terminal* tagSon = new Terminal(scan()->tag);
        son->setChild(tagSon);
        move();
        //terminal
        if(scan()->tag == ID) {
            auto initRes = init(son);
            if(!initRes .status) {
                tokenIterator = last;
                return {false, "init > " + initRes.info};
            }
        } else {
            tokenIterator = last;
            return {false, "ID"};
        }
    } else {
        tokenIterator = last;
        return {false, "(ID | MUL)"};
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::paradata(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(PARADATA);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //terminal
    if(scan()->tag == MUL) {
        Terminal* tagSon = new Terminal(scan()->tag);
        son->setChild(tagSon);
        move();
        //terminal
        if(scan()->tag == ID) {
            Terminal* tagSon = new Terminal(scan()->tag);
            son->setChild(tagSon);
            move();
        } else {
            tokenIterator = last;
            return {false, "ID"}; 
        }
    //terminal
    } else if(scan()->tag == ID) {
        Terminal* tagSon = new Terminal(scan()->tag);
        son->setChild(tagSon);
        move();
        //nonterminal
        auto paradatatailRes = paradatatail(son);
        if(!paradatatailRes.status) {
            tokenIterator = last;
            return {false, "paradatatail > " + paradatatailRes.info};
        }
    } else {
        tokenIterator = last;
        return {false, "(ID | MUL)"};
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::paradatatail(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(PARADATATAIL);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //terminal
    if(scan()->tag == LBRACK) {
        Terminal* tagSon = new Terminal(scan()->tag);
        son->setChild(tagSon);
        move();
        //terminal
        if(scan()->tag == NUM) {
            Terminal* tagSon = new Terminal(scan()->tag);
            son->setChild(tagSon);
            move();
            //terminal
            if(scan()->tag == RBRACK) {
                Terminal* tagSon = new Terminal(scan()->tag);
                son->setChild(tagSon);
                move();
            } else {
                //terminal
                tokenIterator = last;
                
            }
        } else {
            //terminal
            tokenIterator = last;
            
        }
    } else {
        //terminal
        tokenIterator = last;
        
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::paralist(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(PARALIST);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //terminal
    if(scan()->tag == COMMA) {
        Terminal* tagSon = new Terminal(scan()->tag);
        son->setChild(tagSon);
        move();
        //nonterminal
        auto typeRes = type(son);
        if(typeRes.status) {
            //nonterminal
            auto paradataRes = paradata(son);
            if(paradataRes.status) {
                //nonterminal
                auto paralistRes = paralist(son);
                if(!paralistRes.status) {
                    //terminal
                    tokenIterator = last;
                    
                }
            } else {
                //terminal
                tokenIterator = last;
                
            }
        } else {
            //terminal
            tokenIterator = last;
            
        }
    } else {
        //terminal
        tokenIterator = last;
        
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::block(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(BLOCK);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //terminal
    if(scan()->tag == LBRACE) {
        Terminal* tagSon = new Terminal(scan()->tag);
        son->setChild(tagSon);
        move();
        //nonterminal
        auto subprogramRes = subprogram(son);
        if(subprogramRes.status) {
            //terminal
            if(scan()->tag == RBRACE) {
                Terminal* tagSon = new Terminal(scan()->tag);
                son->setChild(tagSon);
                move();
            } else {
                tokenIterator = last;
                return {false, "RBRACE"};
            }
        } else {  
            tokenIterator = last;
            return {false, "subprogram > " + subprogramRes.info}; 
        }
    } else {
        tokenIterator = last;
        return {false, "LBRACE"};
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::assexpr(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(ASSEXPR);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //nonterminal
    auto orexprRes = orexpr(son);
    if(orexprRes.status) {
        //nonterminal
        auto asstailRes = asstail(son);
        if(!asstailRes.status) {
            tokenIterator = last;
            return {false, "asstail > " + orexprRes.info};
        }
    } else {  
        tokenIterator = last;
        return {false, "orexpr > " + orexprRes.info}; 
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::orexpr(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(OREXPR);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //nonterminal
    auto andexprRes = andexpr(son);
    if(andexprRes.status) {
        //nonterminal
        auto ortailRes = ortail(son);
        if(!ortailRes.status) {
            tokenIterator = last;
            return {false, "ortail > " + ortailRes.info};
        }
    } else {  
        tokenIterator = last;
        return {false, "andexpr > " + andexprRes.info}; 
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::andexpr(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(ANDEXPR);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //nonterminal
    auto cmpexprRes = cmpexpr(son);
    if(cmpexprRes.status) {
        //nonterminal
        auto andtailRes = andtail(son);
        if(!andtailRes.status) {
            tokenIterator = last;
            return {false, "andtail > " + andtailRes.info};
        }
    } else {  
        tokenIterator = last;
        return {false, "cmpexpr > " + cmpexprRes.info}; 
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::cmpexpr(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(CMPEXPR);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //nonterminal
    auto aloexprRes = aloexpr(son);
    if(aloexprRes.status) {
        //nonterminal
        auto cmptailRes = cmptail(son);
        if(!cmptailRes.status) {
            tokenIterator = last;
            return {false, "cmptail > " + cmptailRes.info};
        }
    } else {  
        tokenIterator = last;
        return {false, "aloexpr > " + aloexprRes.info}; 
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::aloexpr(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(ALOEXPR);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //nonterminal
    auto itemRes = item(son);
    if(itemRes.status) {
        //nonterminal
        auto alotailRes = alotail(son);
        if(!alotailRes.status) {
            tokenIterator = last;
            return {false, "alotail > " + alotailRes.info};
        }
    } else {  
        tokenIterator = last;
        return {false, "item > " + itemRes.info}; 
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::item(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(ITEM);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //nonterminal
    auto factorRes = factor(son);
    if(factorRes.status) {
        //nonterminal
        auto itemtailRes = itemtail(son);
        if(!itemtailRes.status) {
            tokenIterator = last;
            return {false, "itemtail > " + itemtailRes.info};
        }
    } else {  
        tokenIterator = last;
        return {false, "factor > " + factorRes.info}; 
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
//fix
matchInfo Parser::factor(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(FACTOR);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    bool secondFlag = false;
    //nonterminal
    auto lopRes = lop(son);
    if(lopRes.status) {
        //nonterminal
        auto factorRes = factor(son);
        if(!factorRes.status) {
            tokenIterator = last;
            //nonterminal
            auto valRes = val(son);
            if(!valRes.status) {
                tokenIterator = last;
                secondFlag = true;
            }
        }
    } else {  
        tokenIterator = last;
        secondFlag = true;
    }
    if(secondFlag) {
        //nonterminal
        auto valRes = val(son);
        if(!valRes.status) {
            tokenIterator = last;
            return {false, "(val | lop) > " + valRes.info};
        }
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::asstail(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(ASSTAIL);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //terminal
    if(scan()->tag == ASSIGN) {
        Terminal* tagSon = new Terminal(scan()->tag);
        son->setChild(tagSon);
        move();
        //nonterminal
        auto orexprRes = orexpr(son);
        if(orexprRes.status) {
            //nonterminal
            auto asstailRes = asstail(son);
            if(!asstailRes.status) {
                //terminal
                tokenIterator = last;
                
            }
        } else {
            //terminal
            tokenIterator = last;
            
        }
    } else {
        //terminal
        tokenIterator = last;
        
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::ortail(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(ORTAIL);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //terminal
    if(scan()->tag == OR) {
        Terminal* tagSon = new Terminal(scan()->tag);
        son->setChild(tagSon);
        move();
        //nonterminal
        auto andexprRes = andexpr(son);
        if(andexprRes.status) {
            //nonterminal
            auto ortailRes = ortail(son);
            if(!ortailRes.status) {
                //terminal
                tokenIterator = last;
                
            }
        } else {
            //terminal
            tokenIterator = last;
            
        }
    } else {
        //terminal
        tokenIterator = last;
        
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::andtail(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(ANDTAIL);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //terminal
    if(scan()->tag == AND) {
        Terminal* tagSon = new Terminal(scan()->tag);
        son->setChild(tagSon);
        move();
        //nonterminal
        auto cmpexprRes = cmpexpr(son);
        if(cmpexprRes.status) {
            //nonterminal
            auto andtailRes = andtail(son);
            if(!andtailRes.status) {
                //terminal
                tokenIterator = last;
                
            }
        } else {
            //terminal
            tokenIterator = last;
            
        }
    } else {
        //terminal
        tokenIterator = last;
        
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::cmptail(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(CMPTAIL);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //nonterminal
    auto cmpsRes = cmps(son);
    if(cmpsRes.status) {
        //nonterminal
        auto aloexprRes = aloexpr(son);
        if(aloexprRes.status) {
            //nonterminal
            auto cmptailRes = cmptail(son);
            if(!cmptailRes.status) {
                //terminal
                tokenIterator = last;
                
            }
        } else {
            //terminal
            tokenIterator = last;
            
        }
    } else {
        //terminal
        tokenIterator = last;
        
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::alotail(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(ALOTAIL);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //nonterminal
    auto addsRes = adds(son);
    if(addsRes.status) {
        //nonterminal
        auto itemRes = item(son);
        if(itemRes.status) {
            //nonterminal
            auto alotailRes = alotail(son);
            if(!alotailRes.status) {
                //terminal
                tokenIterator = last;
                
            }
        } else {
            //terminal
            tokenIterator = last;
            
        }
    } else {
        //terminal
        tokenIterator = last;
        
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::itemtail(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(ITEMTAIL);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //nonterminal
    auto mulsRes = muls(son);
    if(mulsRes.status) {
        //nonterminal
        auto factorRes = factor(son);
        if(factorRes.status) {
            //nonterminal
            auto itemtailRes = itemtail(son);
            if(!itemtailRes.status) {
                //terminal
                tokenIterator = last;
                
            }
        } else {
            //terminal
            tokenIterator = last;
            
        }
    } else {
        //terminal
        tokenIterator = last;
        
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::cmps(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(CMPS);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //terminal
    if(scan()->tag == GT || scan()->tag == GE || scan()->tag == LT || scan()->tag == LE || scan()->tag == EQU || scan()->tag == NEQU) {
        Terminal* tagSon = new Terminal(scan()->tag);
        son->setChild(tagSon);
        move();
    } else {
        tokenIterator = last;
        return {false, "(GT | GE | LT | LE | EQU | NEQU)"};
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::adds(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(ADDS);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //terminal
    if(scan()->tag == ADD || scan()->tag == SUB) {
        Terminal* tagSon = new Terminal(scan()->tag);
        son->setChild(tagSon);
        move();
    } else {
        tokenIterator = last;
        return {false, "(ADD | SUB)"};
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::muls(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(MULS);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //terminal
    if(scan()->tag == MUL || scan()->tag == DIV || scan()->tag == MOD) {
        Terminal* tagSon = new Terminal(scan()->tag);
        son->setChild(tagSon);
        move();
    } else {
        tokenIterator = last;
        return {false, "(MUL | DIV | MOD)"};
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::lop(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(CMPS);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //terminal
    if(scan()->tag == NOT || scan()->tag == SUB || scan()->tag == MUL || scan()->tag == INC || scan()->tag == DEC) {
        Terminal* tagSon = new Terminal(scan()->tag);
        son->setChild(tagSon);
        move();
    } else {
        tokenIterator = last;
        return {false, "(NOT | SUB | MUL | INC | DEC)"};
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::rop(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(ROP);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //terminal
    if(scan()->tag == INC || scan()->tag == DEC) {
        Terminal* tagSon = new Terminal(scan()->tag);
        son->setChild(tagSon);
        move();
    } else {
        tokenIterator = last;
        
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::val(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(VAL);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //nonterminal
    auto elemRes = elem(son);
    if(elemRes.status) {
        //nonterminal
        auto ropRes = rop(son);
        if(!ropRes.status) {
            tokenIterator = last;
            return {false, "rop > " + ropRes.info};
        }
    } else {  
        tokenIterator = last;
        return {false, "elem > " + elemRes.info};
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
//switch
//fixed
matchInfo Parser::elem(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(ELEM);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    bool secondFlag = false;
    switch(scan()->tag) {
        //terminal
        case ID: {
            Terminal* tagSon = new Terminal(scan()->tag);
            son->setChild(tagSon);
            move();
            //nonterminal
            auto idexprRes = idexpr(son);
            if(!idexprRes.status) {
                tokenIterator = last;
                secondFlag = true;
            }
            break;
        }
        //terminal
        case LPAREN: {
            Terminal* tagSon = new Terminal(scan()->tag);
            son->setChild(tagSon);
            move();
            //nonterminal
            auto exprRes = expr(son);
            if(exprRes.status) {   
                //terminal
                if(scan()->tag == RPAREN) {
                    Terminal* tagSon = new Terminal(scan()->tag);
                    son->setChild(tagSon);
                    move();
                } else {
                    tokenIterator = last;
                   secondFlag = true;
                }
            } else {
                tokenIterator = last;
                secondFlag = true;
            }
            break;
        }
        //nonterminal
        default: {
            secondFlag = true;
        }
    }
    if(secondFlag) {
        auto literalRes = literal(son);
        if(!literalRes.status) {
            tokenIterator = last;
            return {false, "(ID | LPAREN | literal) > " + literalRes.info};
        }
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::idexpr(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(IDEXPR);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //terminal
    if(scan()->tag == LBRACK) {
        Terminal* tagSon = new Terminal(scan()->tag);
        son->setChild(tagSon);
        move();
        //nonterminal
        auto exprRes = expr(son);
        if(exprRes.status) {
            //terminal
            if(scan()->tag == RBRACK) {
                Terminal* tagSon = new Terminal(scan()->tag);
                son->setChild(tagSon);
                move();
            } else {
                //terminal
                tokenIterator = last;
                
            }
        } else {
            //terminal
            tokenIterator = last;
            
        }
    //terminal
    } else if(scan()->tag == LPAREN) {
        Terminal* tagSon = new Terminal(scan()->tag);
        son->setChild(tagSon);
        move();
        //nonterminal
        auto realargRes = realarg(son);
        if(realargRes.status) {
            //terminal
            if(scan()->tag == RPAREN) {
                Terminal* tagSon = new Terminal(scan()->tag);
                son->setChild(tagSon);
                move();
            } else {
                //terminal
                tokenIterator = last;
                
            }
        } else {
            //terminal
            tokenIterator = last;
            
        }
    } else {
        //terminal
        tokenIterator = last;
        
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::realarg(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(REALARG);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //nonterminal
    auto argRes = arg(son);
    if(argRes.status) {
        //nonterminal
        auto arglistRes = arglist(son);
        if(!arglistRes.status) {
            //terminal
            tokenIterator = last;
            
        }
    } else {
        //terminal
        tokenIterator = last;
        
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::arg(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(ARG);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //nonterminal
    auto exprRes = expr(son);
    if(!exprRes.status) {
        //terminal
        tokenIterator = last;
        return {false, "expr > " + exprRes.info};
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::arglist(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(ARGLIST);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //terminal
    if(scan()->tag == COMMA) {
        Terminal* tagSon = new Terminal(scan()->tag);
        son->setChild(tagSon);
        move();
        //nonterminal
        auto argRes = arg(son);
        if(argRes.status) {
            //nonterminal
            auto arglistRes = arglist(son);
            if(!arglistRes.status) {
                //terminal
                tokenIterator = last;
                
            }
        } else {
            //terminal
            tokenIterator = last;
            
        }
    } else {
        //terminal
        tokenIterator = last;
        
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::literal(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(LITERAL);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //terminal
    if(scan()->tag == NUM || scan()->tag == CH || scan()->tag == STR) {
        Terminal* tagSon = new Terminal(scan()->tag);
        son->setChild(tagSon);
        move();
    } else {
        tokenIterator = last;
        return {false, "(NUM | CH || STR)"};
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
//fixed
matchInfo Parser::subprogram(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(SUBPROGRAM);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    bool secondFlag = false;
    //nonterminal
    auto localdefRes = localdef(son);
    if(localdefRes.status) {
        //nonterminal
        auto subprogramRes = subprogram(son);
        if(!subprogramRes.status) {
            //terminal
            tokenIterator = last;
            secondFlag = true;
        }
    } else {
        tokenIterator = last;
        secondFlag = true;
    }
    if(secondFlag) {
        //nonterminal
        auto statementRes = statement(son);
        if(statementRes.status) {
            //nonterminal
            auto subprogramRes = subprogram(son);
            if(!subprogramRes.status) {
                //terminal
                tokenIterator = last;
                
            }
        } else {
            //terminal
            tokenIterator = last;
               
        }
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::localdef(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(LOCALDEF);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //nonterminal
    auto typeRes = type(son);
    if(typeRes.status) {
        //nonterminal
        auto defdataRes = defdata(son);
        if(defdataRes.status) {
            //nonterminal
            auto deflistRes = deflist(son);
            if(!deflistRes.status) {
                tokenIterator = last;
                return {false, "deflist > " + deflistRes.info};
            }
        } else {
            tokenIterator = last;
            return {false, "defdata > " + defdataRes.info};
        }
    } else {
        tokenIterator = last;
        return {false, "type > " + typeRes.info};
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
//switch
//fixed
matchInfo Parser::statement(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(STATEMENT);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //NO NEED TO USE FLAG
    switch(scan()->tag) {
        //terminal
        case KW_BREAK: {
            Terminal* tagSon = new Terminal(scan()->tag);
            son->setChild(tagSon);
            move();
            //terminal
            if(scan()->tag == SEMICON) {
                Terminal* tagSon = new Terminal(scan()->tag);
                son->setChild(tagSon);
                move();
            } else {
                tokenIterator = last;
                return {false, "SEMICON"};
            }
            break;
        }
        //terminal
        case KW_CONTINUE: {
            Terminal* tagSon = new Terminal(scan()->tag);
            son->setChild(tagSon);
            move();
            //terminal
            if(scan()->tag == SEMICON) {
                Terminal* tagSon = new Terminal(scan()->tag);
                son->setChild(tagSon);
                move();
            } else {
                tokenIterator = last;
                return {false, "SEMICON"};
            }
            break;
        }
        //terminal
        case KW_RETURN: {
            Terminal* tagSon = new Terminal(scan()->tag);
            son->setChild(tagSon);
            move();
            //nonterminal
            auto altexprRes = altexpr(son);
            if(altexprRes.status) {
                //terminal
                if(scan()->tag == SEMICON) {
                    Terminal* tagSon = new Terminal(scan()->tag);
                    son->setChild(tagSon);
                    move();
                } else {
                    tokenIterator = last;
                    return {false, "SEMICON"};
                }
            } else {
                tokenIterator = last;
                return {false, "altexpr > " + altexprRes.info};
            }
            break;
        }
        default: {
            bool secondFlag = false;
            //nonterminal
            auto altexprRes = altexpr(son);
            if(altexprRes.status) {
                //terminal
                if(scan()->tag == SEMICON) {
                    Terminal* tagSon = new Terminal(scan()->tag);
                    son->setChild(tagSon);
                    move();
                } else {
                    tokenIterator = last;
                    secondFlag = true;
                }
            } else {
                //nonterminal
                tokenIterator = last;
                secondFlag = true;
            }
            if(secondFlag) {
                auto whilestatRes = whilestat(son);
                if(!whilestatRes.status) {
                    //nonterminal
                    tokenIterator = last;
                    auto forstatRes = forstat(son);
                    if(!forstatRes.status) {
                        //nonterminal
                        tokenIterator = last;
                        auto dowhilestatRes = dowhilestat(son);
                        if(!dowhilestatRes.status) {
                            //nonterminal
                            tokenIterator = last;
                            auto ifstatRes = ifstat(son);
                            if(!ifstatRes.status) {
                                //nonterminal
                                tokenIterator = last;
                                auto switchstatRes = switchstat(son);
                                if(!switchstatRes.status) {
                                    tokenIterator = last;
                                    return {false, "(KW_BREAK | KW_CONTINUE | KW_RETURN | altexpr | whilestat | forstat | dowhilestat | ifstat | switchstat)"};
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::whilestat(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(WHILESTAT);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;
    
    //terminal
    if(scan()->tag == KW_WHILE) {
        symTab.enter();
        Terminal* tagSon = new Terminal(scan()->tag);
        son->setChild(tagSon);
        move();
        //terminal
        if(scan()->tag == LPAREN) {
            Terminal* tagSon = new Terminal(scan()->tag);
            son->setChild(tagSon);
            move();
            //nonterminal
            auto altexprRes = altexpr(son);
            if(altexprRes.status) {
                //terminal
                if(scan()->tag == RPAREN) {
                    Terminal* tagSon = new Terminal(scan()->tag);
                    son->setChild(tagSon);
                    move();
                    //nonterminal
                    auto blockRes = block(son);
                    if(!blockRes.status) {
                        tokenIterator = last;
                        return {false, "block > " + blockRes.info};
                    }
                } else {
                    tokenIterator = last;
                    return {false, "RPAREN"};
                }
            } else {
                tokenIterator = last;
                return {false, "altexpr > " + altexprRes.info};
            }
        } else {
            tokenIterator = last;
            return {false, "LPAREN"};
        }
        symTab.leave();
    } else {
        tokenIterator = last;
        return {false, "KW_WHILE"};
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::dowhilestat(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(DOWHILESTAT);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    symTab.enter();
    //terminal
    if(scan()->tag == KW_DO) {
        Terminal* tagSon = new Terminal(scan()->tag);
        son->setChild(tagSon);
        move();
        //nonterminal
        auto blockRes = block(son);
        if(blockRes.status) {
            //terminal
            if(scan()->tag == KW_WHILE) {
                Terminal* tagSon = new Terminal(scan()->tag);
                son->setChild(tagSon);
                move();
                //terminal
                if(scan()->tag == LPAREN) {
                    Terminal* tagSon = new Terminal(scan()->tag);
                    son->setChild(tagSon);
                    move();
                    symTab.leave();
                    //nonterminal
                    auto altexprRes = altexpr(son);
                    if(altexprRes.status) {
                        //terminal
                        if(scan()->tag == RPAREN) {
                            Terminal* tagSon = new Terminal(scan()->tag);
                            son->setChild(tagSon);
                            move();
                            //terminal
                            if(scan()->tag == SEMICON) {
                                Terminal* tagSon = new Terminal(scan()->tag);
                                son->setChild(tagSon);
                                move();
                            } else {
                                tokenIterator = last;
                                return {false, "SEMICON"};
                            }
                        } else {
                            tokenIterator = last;
                            return {false, "RPAREN"};
                        }
                    } else {
                        tokenIterator = last;
                        return {false, "altexpr > " + altexprRes.info};
                    }
                } else {
                    tokenIterator = last;
                    return {false, "LPAREN"};
                }
            } else {
                tokenIterator = last;
                return {false, "KW_WHILE"};
            }
        } else {
            tokenIterator = last;
            return {false, "block > " + blockRes.info};
        }
    } else {
        tokenIterator = last;
        return {false, "KW_DO"};
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::forstat(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(FORSTAT);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    
    //terminal
    if(scan()->tag == KW_FOR) {
        symTab.enter();
        Terminal* tagSon = new Terminal(scan()->tag);
        son->setChild(tagSon);
        move();
        //terminal
        if(scan()->tag == LPAREN) {
            Terminal* tagSon = new Terminal(scan()->tag);
            son->setChild(tagSon);
            move();
            //nonterminal
            auto forinitRes = forinit(son);
            if(forinitRes.status) {
                //nonterminal
                auto altexprRes = altexpr(son);
                if(altexprRes.status) {
                    //terminal
                    if(scan()->tag == SEMICON) {
                        Terminal* tagSon = new Terminal(scan()->tag);
                        son->setChild(tagSon);
                        move();
                        //nonterminal
                        auto altexpr2Res = altexpr(son);
                        if(altexpr2Res.status) {
                            //terminal
                            if(scan()->tag == RPAREN) {
                                Terminal* tagSon = new Terminal(scan()->tag);
                                son->setChild(tagSon);
                                move();
                                //nonterminal
                                auto blockRes = block(son);
                                if(!blockRes.status) {
                                    tokenIterator = last;
                                    return {false, "block > " + blockRes.info};
                                }
                            } else {
                                tokenIterator = last;
                                return {false, "RPAREN"};
                            }
                        } else {
                            tokenIterator = last;
                            return {false, "(second)altexpr > " + altexpr2Res.info};
                        }
                    } else {
                        tokenIterator = last;
                        return {false, "SEMICON"};
                    }
                } else {
                    tokenIterator = last;
                    return {false, "altexpr > " + altexprRes.info};
                }
            } else {
                tokenIterator = last;
                return {false, "forinit > " + forinitRes.info};
            }
        } else {
            tokenIterator = last;
            return {false, "LPAREN"};
        }
        symTab.leave();
    } else {
        tokenIterator = last;
        return {false, "KW_FOR"};
    }
    

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::forinit(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(FORINIT);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //nonterminal
    auto localdefRes = localdef(son);
    if(!localdefRes.status) {
        tokenIterator = last;
        //nonterminal
        auto altexprRes = altexpr(son);
        if(altexprRes.status) {
            //terminal
            if(scan()->tag == SEMICON) {
                Terminal* tagSon = new Terminal(scan()->tag);
                son->setChild(tagSon);
                move(); 
            } else {
                //terminal
                tokenIterator = last;
                     
            }
        } else {
            //terminal
            tokenIterator = last;
            
        }
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::ifstat(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(IFSTAT);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //terminal
    if(scan()->tag == KW_IF) {
        symTab.enter();
        Terminal* tagSon = new Terminal(scan()->tag);
        son->setChild(tagSon);
        move();
            //terminal
            if(scan()->tag == LPAREN) {
                Terminal* tagSon = new Terminal(scan()->tag);
                son->setChild(tagSon);
                move();
                //nonterminal
                auto exprRes = expr(son);
                if(exprRes.status) {
                    //terminal
                    if(scan()->tag == RPAREN) {
                        Terminal* tagSon = new Terminal(scan()->tag);
                        son->setChild(tagSon);
                        move();
                        //nonterminal
                        auto blockRes = block(son);
                        if(blockRes.status) {
                            //nonterminal
                            auto elsestatRes = elsestat(son);
                            if(!elsestatRes.status) {
                                tokenIterator = last;
                                return {false, "elsestat > " + elsestatRes.info};
                            }
                        } else {
                            tokenIterator = last;
                            return {false, "block > " + blockRes.info};
                        }
                    } else {
                        tokenIterator = last;
                        return {false, "RPAREN"};
                    }
                } else {
                    tokenIterator = last;
                    return {false, "expr > " + exprRes.info};
                }
            } else {
                tokenIterator = last;
                return {false, "LPAREN"};
            }
        symTab.leave();
    } else {
        tokenIterator = last;
        return {false, "KW_IF"};
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::elsestat(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(ELSESTAT);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //terminal
    if(scan()->tag == KW_ELSE) {
        symTab.enter();
        Terminal* tagSon = new Terminal(scan()->tag);
        son->setChild(tagSon);
        move(); 
        //nonterminal
        auto blockRes = block(son);
        if(!blockRes.status) {
            //terminal
            tokenIterator = last;
        }
        symTab.leave();
    } else {
        //terminal
        tokenIterator = last;
             
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::switchstat(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(SWITCHSTAT);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //terminal
    if(scan()->tag == KW_SWITCH) {
        symTab.enter();
        Terminal* tagSon = new Terminal(scan()->tag);
        son->setChild(tagSon);
        move();
        //terminal
        if(scan()->tag == LPAREN) {
            Terminal* tagSon = new Terminal(scan()->tag);
            son->setChild(tagSon);
            move();
            //nonterminal
            auto exprRes = expr(son);
            if(exprRes.status) {
                //terminal
                if(scan()->tag == RPAREN) {
                    Terminal* tagSon = new Terminal(scan()->tag);
                    son->setChild(tagSon);
                    move();
                    //terminal
                    if(scan()->tag == LBRACE) {
                        Terminal* tagSon = new Terminal(scan()->tag);
                        son->setChild(tagSon);
                        move();
                        //nonterminal
                        auto casestatRes = casestat(son);
                        if(casestatRes.status) {
                            //terminal
                            if(scan()->tag == RBRACE) {
                                Terminal* tagSon = new Terminal(scan()->tag);
                                son->setChild(tagSon);
                                move();
                            } else {
                                tokenIterator = last;
                                return {false, "RBRACE"};
                            }
                        } else {
                            tokenIterator = last;
                            return {false, "casestat > " + casestatRes.info};
                        }
                    } else {
                        tokenIterator = last;
                        return {false, "LBRACE"};
                    }

                } else {
                    tokenIterator = last;
                    return {false, "RPAREN"};
                }
            } else {
                tokenIterator = last;
                return {false, "expr > " + exprRes.info};
            }
        } else {
            tokenIterator = last;
            return {false, "LPAREN"};
        }
        symTab.leave();
    } else {
        tokenIterator = last;
        return {false, "KW_SWITCH"};
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
//switch
matchInfo Parser::casestat(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(CASESTAT);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    switch(scan()->tag) {
        //terminal
        case KW_CASE: {
            Terminal* tagSon = new Terminal(scan()->tag);
            son->setChild(tagSon);
            move();
            //nonterminal
            auto caselabelRes = caselabel(son);
            if(caselabelRes.status) {
                if(scan()->tag == COLON) {
                    auto subprogramRes = subprogram(son);
                    if(subprogramRes.status) {
                        auto casestatRes = casestat(son);
                        if(!casestatRes.status) {
                            tokenIterator = last;
                            return {false, "casestat > " + casestatRes.info};
                        }
                    } else {
                        tokenIterator = last;
                        return {false, "subprogram > " + subprogramRes.info};
                    }
                } else {
                    tokenIterator = last;
                    return {false, "COLON"};  
                }
            } else {
                tokenIterator = last;
                return {false, "caselabel > " + caselabelRes.info};
            }
            break;
        }
        //terminal
        case KW_DEFAULT: {
            Terminal* tagSon = new Terminal(scan()->tag);
            son->setChild(tagSon);
            move();
            //terminal
            if(scan()->tag == COLON) {
                Terminal* tagSon = new Terminal(scan()->tag);
                son->setChild(tagSon);
                move();
                auto subprogramRes = subprogram(son);
                if(!subprogramRes.status) {
                    tokenIterator = last;
                    return {false, "subprogram > " + subprogramRes.info};
                }
            } else {
                tokenIterator = last;
                return {false, "COLON"};
            }
            break;
        }
        default: {
            tokenIterator = last;
            return {false, "(KW_CASE | KW_DEFAULT)"};
        }
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}
matchInfo Parser::caselabel(Nonterminal* father) {
    //如果词记号到头，则直接返回
    if(scan() == NULL) return {true, "over"};  
    //创建本层节点
    Nonterminal* son = new Nonterminal(CASELABEL);
    //创建历史纪录，以便出现匹配失败时回溯到匹配前
    list<Token*>::iterator last = tokenIterator;
    //cout<<son->toString()<<endl;

    //nonterminal
    auto literalRes = literal(son);
    if(!literalRes.status) {
        tokenIterator = last;
        return {false, "literal > " + literalRes.info}; 
    }

    //匹配成功，装载节点
    father->setChild(son);
    return {true, ""};
}