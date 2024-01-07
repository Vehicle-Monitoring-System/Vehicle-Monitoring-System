import classNames from 'classnames/bind';
import { Link } from 'react-router-dom';

import images from '../../../assets/images';
import config from '../../../config';
import styles from './Header.module.scss';

const cx = classNames.bind(styles);

function Header() {
    return (
        <header className={cx('wrapper')}>
            <div className={cx('inner')}>
                <Link to={config.routes.home} className={cx('logo-link')}>
                    <img src={images.logo} alt="30sVideo" />
                </Link>

                <Link to={config.routes.home} className={cx('item')}>
                    Giám sát trực tuyến
                </Link>

                <Link to={config.routes.history} className={cx('item')}>
                    Lịch sử chuyến đi
                </Link>
            </div>
        </header>
    );
}

export default Header;
