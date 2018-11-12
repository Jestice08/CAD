/**
 * @file   DefDataBase.h
 * @brief  Database for Def parser
 * @author Yibo Lin
 * @date   Oct 2014
 */

#ifndef DEFPARSER_DATABASE_H
#define DEFPARSER_DATABASE_H

#include <string.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include "../../../include/thirdparty/lefdef/def/include/defrReader.hpp"
#include "../../../include/thirdparty/lefdef/def/include/defiAlias.hpp"

/// namespace for DefParser
namespace DefParser {

/// @nowarn
    using std::cout;
    using std::endl;
    using std::cerr;
    using std::string;
    using std::vector;
    using std::pair;
    using std::make_pair;
    using std::ostringstream;
    typedef int int32_t;
    typedef unsigned int uint32_t;
    typedef long int64_t;
/// @endnowarn

/// @brief Temporary data structures to hold parsed data.
/// Base class for all temporary data structures.
    struct Item
    {
        /// print data members
        virtual void print(ostringstream&) const {};
        /// print data members with stream operator
        /// @param os output stream
        /// @param rhs target object
        /// @return output stream
        friend std::ostream& operator<<(std::ostream& os, Item const& rhs)
        {
            std::ostringstream ss;
            rhs.print(ss);
            os << ss.str();
            return os;
        }
        /// print data members with stream operator
        /// @param ss output stream
        /// @param rhs target object
        /// @return output stream
        friend ostringstream& operator<<(ostringstream& ss, Item const& rhs)
        {
            rhs.print(ss);
            return ss;
        }
    };
/// @brief placement row
    struct Row : public Item
    {
        string row_name; ///< row name
        string macro_name; ///< macro name of row
        int32_t origin[2]; ///< x, y of origin
        string orient; ///< orientation
        int32_t repeat[2]; ///< do x by y
        int32_t step[2]; ///< x, y
        /// @brief reset all data members
        void reset()
        {
            row_name = macro_name = orient = "";
            origin[0] = origin[1]  = -1;
            repeat[0] = repeat[1] = -1;
            step[0] = step[1] = -1;
        }
        /// @brief print data members
        /// @param ss output stream
        virtual void print(ostringstream& ss) const
        {
            ss << "//////// Row ////////" << endl
               << "row_name = " << row_name << endl
               << "macro_name = " << macro_name << endl
               << "origin = " << origin[0] << " " << origin[1] << endl
               << "orient = " << orient << endl
               << "repeat = " << repeat[0] << " " << repeat[1] << endl
               << "step = " << step[0] << " " << step[1] << endl;
        }
    };
/// @brief cell in placement
    struct Component : public Item
    {
        string comp_name; ///< component name
        string macro_name; ///< macro name of component, standard cell type
        string status; ///< placement status
        int32_t origin[2]; ///< x, y of origin
        string orient; ///< orientation
        string regionName;
        int region_size;
        int *xl_r, *yl_r, *xh_r, *yh_r;
        bool ifregion;
        /// @brief reset all data members
        void reset()
        {
            regionName = comp_name = macro_name = status = orient = "";
            region_size = origin[0] = origin[1] = -1;
        }
        /// @brief print data members
        /// @param ss output stream
        virtual void print(ostringstream& ss) const
        {
            ss << "//////// Component ////////" << endl
               << "comp_name = " << comp_name << endl
               << "macro_name = " << macro_name << endl
               << "status = " << status << endl
               << "origin = " << origin[0] << " " << origin[1] << endl
               << "orient = " << orient << endl;
        }
    };
/// @brief pin of node/cell
    struct Pin : public Item
    {
        string pin_name; ///< pin name
        string net_name; ///< net name
        string direct; ///< direction
        string status; ///< placement status
        int32_t origin[2]; ///< offset to node origin
        string orient; ///< orientation
        vector<string> vLayer; ///< layers
        vector<vector<int32_t> > vBbox; ///< bounding box on each layer
        string use; ///< "use" token in DEF file
        /// @brief reset all data members
        void reset()
        {
            pin_name = net_name = direct = status = orient = "";
            origin[0] = origin[1] = -1;
            vLayer.clear();
            vBbox.clear();
            use = "";
        }
        /// @brief print data members
        /// @param ss output stream
        virtual void print(ostringstream& ss) const
        {
            ss << "//////// Pin ////////" << endl
               << "pin_name = " << pin_name << endl
               << "net_name = " << net_name << endl
               << "direct = " << direct << endl
               << "status = " << status << endl
               << "origin = " << origin[0] << " " << origin[1] << endl
               << "orient = " << orient << endl;
            for (uint32_t i = 0; i < vLayer.size(); ++i)
                ss << "layer " << vLayer[i] << " " << vBbox[i][0] << " " << vBbox[i][1] << " " << vBbox[i][2] << " " << vBbox[i][3] << endl;
            ss << "use = " << use << endl;
        }
    };
/// @brief net to describe interconnection of netlist
    struct Net : public Item
    {
        string net_name; ///< net name
        vector< std::pair<string, string> > vNetPin; ///< array of (node, pin) pair
        /// @brief reset all data members
        void reset()
        {
            net_name = "";
            vNetPin.clear();
        }
        /// @brief print data members
        /// @param ss output stream
        virtual void print(ostringstream& ss) const
        {
            ss << "//////// Net ////////" << endl
               << "net_name = " << net_name << endl;
            for (uint32_t i = 0; i < vNetPin.size(); ++i)
                ss << "(" << vNetPin[i].first << ", " << vNetPin[i].second << ") ";
            ss << endl;
        }
    };

    struct Track : public Item
    {
        string macro_name;
        int x;
        int xNum;
        int xStep;
        vector<string> layer;
        void reset(){
            macro_name = "";
            layer.clear();
            x = 0;
            xNum = 0;
            xStep = 0;
        }

    };



    struct Snetp : public Item
    {
        string name; ///< net name
        string wiretype;
        vector<string> layers;
        vector<int> xl; ///
        vector<int> xh;
        vector<int> yl;
        vector<int> yh;
        vector<bool> isshape;
        vector<bool> isvia;
        vector<string> compname; /// Shape name or via name
        vector<int> pathwidth;
        vector<string> vianame;
        /// @brief reset all data members
        void reset()
        {
            name, wiretype = "";
            layers.resize(0);
            xl.resize(0);
            xh.resize(0);
            yl.resize(0);
            yh.resize(0);
            isshape.resize(0);
            isvia.resize(0);
            compname.resize(0);
            pathwidth.resize(0);
            vianame.resize(0);
        }
        /// @brief print data members
        /// @param ss output stream
        virtual void print(ostringstream& ss) const
        {
            ss << "//////// Specialnet ////////" << endl
               << "name = " << name << endl;
            for (int i = 0; i< layers.size(); ++i) {
                if (isshape[i]) {
                    ss<<"shape " <<compname[i]<<" ";
                }
                if (isvia[i]) {
                    ss<<"via "<<vianame[i]<<" ";
                }

                ss<<"width: "<< pathwidth[i]<<" ";
                ss<<" "<<layers[i]<<" "<<xl[i]<<" "<< yl[i]<<" "<< xh[i]<<" "<< yh[i]<<endl;
            }

        }
    };

    struct Region : public  Item
    {
        string name;
        string type;
        vector<int> xl; ///
        vector<int> xh;
        vector<int> yl;
        vector<int> yh;
        void reset()
        {
            name = "";
            type = "UNDEF";
            xl.resize(0);
            xh.resize(0);
            yl.resize(0);
            yh.resize(0);
        }
        /// @brief print data members
        /// @param ss output stream
        virtual void print(ostringstream& ss) const
        {
            ss << "//////// region ////////" << endl
               << "name = " << name << endl;

            ss<<"type"<< type<<endl;
            for (int i=0; i < xl.size(); ++i) {
                ss<<xl[i]<<" "<< xh[i]<<" "<< yl[i]<<" "<< yh[i]<<endl;
            }

        }

    };

    struct Group : public  Item
    {
        string name;
        vector<string> group_members;
        vector<string> regions;
        void reset()
        {
            name = "";
            group_members.resize(0);
            regions.resize(0);
        }
        /// @brief print data members
        /// @param ss output stream
        virtual void print(ostringstream& ss) const
        {
            ss << "//////// group ////////" << endl
               << "name = " << name << endl;
            ss<<"Group members: "<<endl;
            for (int i = 0; i< group_members.size(); ++i) {
                cout<<group_members[i]<<endl;
            }
            ss<<"END OF PRINT"<<endl;

        }

    };

    struct Via : public Item {
        string name;
        vector<string> layers;
        vector<int> xl;
        vector<int> yl;
        vector<int> xh;
        vector<int> yh;

        void reset() {
            name = "";
            layers.resize(0);
            xl.resize(0);
            yl.resize(0);
            xh.resize(0);
            yh.resize(0);
        }

        virtual void print(ostringstream &ss) const {
            ss << "Via " << name << endl;
        }
    };
// forward declaration
/// @class DefParser::DefDataBase
/// @brief Base class for def database.
/// Only pure virtual functions are defined.
/// User needs to inheritate this class and derive a custom database type with all callback functions defined.
    class DefDataBase
    {
    public:
        /// @brief set divider characters
        virtual void set_def_dividerchar(string const&) = 0;
        /// @brief set BUS bit characters
        virtual void set_def_busbitchars(string const&) = 0;
        /// @brief set DEF version
        virtual void set_def_version(string const&) = 0;
        /// @brief set design name
        virtual void set_def_design(string const&) = 0;
        /// @brief set DEF unit
        virtual void set_def_unit(int) = 0;
        /// @brief set die area xl, yl, xh, yh
        virtual void set_def_diearea(int, int, int, int) = 0;
        /// @brief add row
        virtual void add_def_row(Row const&) = 0;
        /// @brief add component/cell
        virtual void add_def_component(Component const&) = 0;
        /// @brief add component/cell
        virtual void resize_def_component(int) = 0;
        /// @brief add pin
        virtual void add_def_pin(Pin const&) = 0;
        /// @brief set number of pins
        virtual void resize_def_pin(int)  = 0;
        /// @brief add net
        virtual void add_def_net(Net const&) = 0;
        /// @brief set number of nets
        virtual void resize_def_net(int) = 0;
        // check these callbacks at runtime
        /// @brief set number of blockages
        virtual void resize_def_blockage(int);
        /// @brief add placement blockages, array of boxes with xl, yl, xh, yh
        virtual void add_def_placement_blockage(std::vector<std::vector<int> > const&);
        /// @brief end of design
        virtual void end_def_design();
        ///
        virtual void add_def_track(Track const&) = 0;
        virtual void add_def_snetpath(Snetp const&) = 0;

        virtual void add_def_region(Region const&) = 0;

        virtual void add_def_group(Group const&) = 0;

        virtual void add_def_via(Via const &) = 0;

    protected:
        /// @brief remind users to define some optional callback functions at runtime
        /// @param str message including the information to the callback function in the reminder
        void def_user_cbk_reminder(const char* str) const;
    };

} // namespace DefParser

#endif
